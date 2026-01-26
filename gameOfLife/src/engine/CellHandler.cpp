#include "CellHandler.hpp"
#include "vec2.hpp"
#include <cstring>
#include <fstream>
#include <iostream>

namespace life {

// tämä structi on sama kuin se toinen, koska memcpy ei halunnut toimia sen
// kanssa.
// Virhe: ”void* memcpy(void*, const void*, size_t)” kirjoittaa
// ei-triviaalisti-kopioitavaan tyyppiin ”life::Vec2u16” {aka ”struct
// life::Vec<2, short unsigned int>”}; use copy-assignment or
// copy-initialization instead
// vähä röpö, mutta ei
struct Vec2u16_t {
  uint16_t x;
  uint16_t y;
};

uint32_t vec2ToIndex(Vec2u16 position, uint16_t rowSize) {
  return static_cast<uint32_t>((position.y * rowSize) + position.x);
}

CellHandler::CellHandler(Vec2u16 gridSize) : gridSize(gridSize) {
  const uint32_t size = getSize();
  futureGrid.resize(size, false);
  currentGrid.resize(size, false);
}

// Ohjeissa luki, että esimmäinen rivi tiedostossa pitää olla sen nimi, mutta
// olemme sen verran erilaisempia, että siirrämme sen nimen tiedosto nimeksi.
// Täten ensimmäinen rivi tulee olemaan peli alueen x ja y koko. Sitten vasta
// kaikki muu :D
bool CellHandler::loadFromFile(std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  file.seekg(0, std::ios::end);
  const size_t fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  if (fileSize <= 0) {
    return false;
  }

  std::vector<char> buffer(fileSize);
  file.read(buffer.data(), fileSize);

  Vec2u16_t newGridSize;
  memcpy(&newGridSize, buffer.data(), sizeof(Vec2u16));
  gridSize.x = newGridSize.x;
  gridSize.y = newGridSize.y;

  const uint32_t newGridArraySize = getSize();
  futureGrid.resize(newGridArraySize, false);
  currentGrid.resize(newGridArraySize, false);

  // skippa gridSize ja ensimmäinen \n char
  size_t offset = sizeof(Vec2u16) + 1;
  Vec2u16 position{0, 0};

  for (size_t i = offset; i < buffer.size(); ++i) {
    char c = buffer[i];
    if (c == '\n') {
      position.y++;
      position.x = 0;
      continue;
    }

    if (c == 'x') {
      setTile(position);
    }
    position.x++;
  }

  return true;
}

bool CellHandler::saveFile(std::string &filename) {
  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    return false;
  }
  const char *ptr = reinterpret_cast<const char *>(&gridSize);
  file.write(ptr, sizeof(Vec2u16));

  file.put('\n');

  for (uint16_t row = 0; row < gridSize.y; ++row) {
    for (uint16_t col = 0; col < gridSize.x; ++col) {
      Vec2u16 pos(col, row);
      bool alive = getCellState(pos);
      file.put(alive ? 'x' : ' ');
    }
    file.put('\n');
  }

  return true;
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
