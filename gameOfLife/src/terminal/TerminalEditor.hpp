#pragma once
#include "../engine/CellHandler.hpp"
#include "../engine/vec2.hpp"

namespace life {

class TerminalEditor {
public:
  TerminalEditor(CellHandler &handler);
  ~TerminalEditor();

  void run();

private:
  void render();
  void handleInput();
  void moveCursor(Vec2i16 direction);
  void toggleCell();
  void clearScreen();
  bool hasAliveCells();
  bool checkInput(int timeoutMs);

  void enableRawMode();
  void disableRawMode();

  CellHandler &handler;
  Vec2u16 cursorPos;
  Vec2u16 gridSize;
  bool running;
  bool autoRunning;
  uint32_t generation;
  uint32_t delayMs;
};

} // namespace life
