#pragma once
#include "../engine/vec2.hpp"

namespace life {
class ArgumentParser {
public:
  ArgumentParser(int argc, char *argv[]);
  Vec2u16 getGridSize() const;

private:
  uint32_t getNumLen(char *str);
  uint16_t u16FromStr(char *str);

private:
  Vec2u16 gridSize;
};

} // namespace life
