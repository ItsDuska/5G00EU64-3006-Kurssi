#include "CellHandler.hpp"
#include "vec2.hpp"
#include <iostream>

namespace life {

uint32_t vec2ToIndex(Vec2u16 position, uint16_t rowSize) {
  return static_cast<uint32_t>((position.y * rowSize) + position.x);
}

CellHandler::CellHandler(Vec2u16 gridSize) : gridSize(gridSize) {
  const uint32_t size = getSize();
  futureGrid.resize(size, false);
  currentGrid.resize(size, false);
  std::cout << "x: " << gridSize.x << "\n" << "y: " << gridSize.y << "\n";
}

void CellHandler::setTile(Vec2u16 position) {
  const uint32_t tmp = vec2ToIndex(position, gridSize.x);
  this->currentGrid[tmp] = !this->currentGrid[tmp];
}

uint32_t CellHandler::countNeighbours(Vec2u16 position) {
  uint32_t count = 0;

  for (int16_t directionY = -1; directionY <= 1; ++directionY) {
    for (int16_t directionX = -1; directionX <= 1; ++directionX) {
      if (directionX == 0 && directionY == 0) {
        continue;
      }

      int16_t newX = static_cast<int16_t>(position.x) + directionX;
      int16_t newY = static_cast<int16_t>(position.y) + directionY;

      if (newX >= 0 && newX < static_cast<int16_t>(gridSize.x) && newY >= 0 &&
          newY < static_cast<int16_t>(gridSize.y)) {
        Vec2u16 neighborPos(static_cast<uint16_t>(newX),
                            static_cast<uint16_t>(newY));
        uint32_t index = vec2ToIndex(neighborPos, gridSize.x);
        if (currentGrid[index]) {
          ++count;
        }
      }
    }
  }

  return count;
}

void CellHandler::update() {
  for (uint16_t y = 0; y < gridSize.y; ++y) {
    for (uint16_t x = 0; x < gridSize.x; ++x) {
      Vec2u16 position(x, y);
      uint32_t index = vec2ToIndex(position, gridSize.x);
      uint32_t neighbors = countNeighbours(position);
      bool isAlive = currentGrid[index];

      if (isAlive) {
        futureGrid[index] = (neighbors == 2 || neighbors == 3);
      } else {
        futureGrid[index] = (neighbors == 3);
      }
    }
  }

  currentGrid.swap(futureGrid);
}

bool CellHandler::getCellState(Vec2u16 position) const {
  uint32_t index = vec2ToIndex(position, gridSize.x);
  return currentGrid[index];
}

void CellHandler::clear() {
  std::fill(currentGrid.begin(), currentGrid.end(), false);
  std::fill(futureGrid.begin(), futureGrid.end(), false);
}

} // namespace life
