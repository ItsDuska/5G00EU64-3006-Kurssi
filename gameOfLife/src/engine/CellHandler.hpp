#pragma once
#include "vec2.hpp"
#include <cstdint>
#include <vector>

namespace life {
uint32_t vec2ToIndex(Vec2u16 position, uint16_t rowSize);

class CellHandler {
public:
  CellHandler(Vec2u16 gridSize);
  ~CellHandler() = default;

  void setTile(Vec2u16 position);

  void update();

  uint32_t countNeighbours(Vec2u16 position);
  bool getCellState(Vec2u16 position) const;

  void clear();

  uint32_t getSize() const {
    return static_cast<uint32_t>(gridSize.x * gridSize.y);
  }

  Vec2u16 getGridSize() const { return gridSize; }

private:
  std::vector<bool> currentGrid;
  std::vector<bool> futureGrid;
  Vec2u16 gridSize;
};

} // namespace life
