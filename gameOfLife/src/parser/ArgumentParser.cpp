#include "ArgumentParser.hpp"
#include <cctype>
#include <charconv>
#include <cstring>

namespace life {
uint32_t ArgumentParser::getNumLen(char *str) {
  char *currentPosition = str;
  const char *start = str;
  while (std::isdigit(static_cast<int>(*currentPosition)) != 0) {
    currentPosition++;
  }
  return static_cast<uint32_t>(currentPosition - start);
}

uint16_t ArgumentParser::u16FromStr(char *str) {
  uint16_t result = 0;
  uint32_t len = getNumLen(str);
  std::from_chars(str, str + len, result);
  return result;
}

ArgumentParser::ArgumentParser(int argc, char *argv[]) {
  /*
   * id 1 = x u16
   * id 2 = y u16
   */

  if (argc < 3) {
    // käytä default arvoja
    this->gridSize = {60, 20};
    return;
  }
  this->gridSize.x = u16FromStr(argv[1]);
  this->gridSize.y = u16FromStr(argv[2]);
}

Vec2u16 ArgumentParser::getGridSize() const { return gridSize; }
} // namespace life
