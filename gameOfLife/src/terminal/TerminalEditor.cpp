#include "TerminalEditor.hpp"
#include <chrono>
#include <cstring>
#include <iostream>
#include <sys/select.h>
#include <termios.h>
#include <thread>
#include <unistd.h>
namespace life {

constexpr Vec2i16 DIRECTION_UP = Vec2i16(0, -1);
constexpr Vec2i16 DIRECTION_DOWN = Vec2i16(0, 1);
constexpr Vec2i16 DIRECTION_RIGHT = Vec2i16(1, 0);
constexpr Vec2i16 DIRECTION_LEFT = Vec2i16(-1, 0);

static struct termios originalTermios;

TerminalEditor::TerminalEditor(CellHandler &handler)
    : handler(handler), cursorPos(0, 0), running(true), autoRunning(false),
      generation(0), delayMs(200) {
  gridSize = handler.getGridSize();
}

TerminalEditor::~TerminalEditor() { disableRawMode(); }

void TerminalEditor::enableRawMode() {
  tcgetattr(STDIN_FILENO, &originalTermios);

  struct termios raw = originalTermios;
  raw.c_lflag &= ~(ECHO | ICANON);
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void TerminalEditor::disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios);
}

void TerminalEditor::clearScreen() {
  std::cout << "\033[2J\033[H" << std::flush;
}

void TerminalEditor::render() {
  clearScreen();

  std::cout << "Game of Life Editor\n";
  std::cout << "Generation: " << generation;
  std::cout << " | Position: (" << cursorPos.x << ", " << cursorPos.y << ")";
  std::cout << " | Mode: " << (autoRunning ? "AUTO-RUN" : "PAUSED") << "\n\n";

  for (uint16_t y = 0; y < gridSize.y; ++y) {
    for (uint16_t x = 0; x < gridSize.x; ++x) {
      Vec2u16 pos(x, y);
      bool isAlive = handler.getCellState(pos);
      bool isCursor = (x == cursorPos.x && y == cursorPos.y);

      if (isCursor) {
        std::cout << "\033[7m";
      }

      std::cout << (isAlive ? '#' : '.');

      if (isCursor) {
        std::cout << "\033[0m";
      }
    }
    std::cout << '\n';
  }

  std::cout
      << "\nControls: Arrow keys (move) | Space (toggle) | P (play/pause) | ";
  std::cout << "R (step) | C (clear) | +/- (speed) | Q (quit)\n";
  std::cout << std::flush;
}

void TerminalEditor::moveCursor(Vec2i16 direction) {
  int16_t newX = static_cast<int16_t>(cursorPos.x) + direction.x;
  int16_t newY = static_cast<int16_t>(cursorPos.y) + direction.y;

  if (newX < 0)
    newX = 0;
  if (newX >= static_cast<int16_t>(gridSize.x))
    newX = gridSize.x - 1;
  if (newY < 0)
    newY = 0;
  if (newY >= static_cast<int16_t>(gridSize.y))
    newY = gridSize.y - 1;

  cursorPos.x = static_cast<uint16_t>(newX);
  cursorPos.y = static_cast<uint16_t>(newY);
}
void TerminalEditor::toggleCell() { handler.setTile(cursorPos); }

bool TerminalEditor::hasAliveCells() {
  for (uint16_t y = 0; y < gridSize.y; ++y) {
    for (uint16_t x = 0; x < gridSize.x; ++x) {
      if (handler.getCellState(Vec2u16(x, y))) {
        return true;
      }
    }
  }
  return false;
}

bool TerminalEditor::checkInput(int timeoutMs) {
  fd_set readfds;
  struct timeval tv;

  FD_ZERO(&readfds);
  FD_SET(STDIN_FILENO, &readfds);

  tv.tv_sec = timeoutMs / 1000;
  tv.tv_usec = (timeoutMs % 1000) * 1000;

  int result = select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &tv);
  return result > 0;
}
void TerminalEditor::handleInput() {
  char c;
  ssize_t nread = read(STDIN_FILENO, &c, 1);

  if (nread <= 0) {
    return;
  }

  if (c == '\033') {
    char seq[2];
    if (read(STDIN_FILENO, &seq[0], 1) != 1)
      return;
    if (read(STDIN_FILENO, &seq[1], 1) != 1)
      return;

    if (seq[0] == '[') {
      switch (seq[1]) {
      case 'A':
        moveCursor(DIRECTION_UP);
        break;
      case 'B':
        moveCursor(DIRECTION_DOWN);
        break;
      case 'C':
        moveCursor(DIRECTION_RIGHT);
        break;
      case 'D':
        moveCursor(DIRECTION_LEFT);
        break;
      }
    } else {
      running = false;
    }
  } else {
    switch (c) {
    case 'q':
      running = false;
      break;

    case ' ':
      toggleCell();
      break;

    case 'w':
      moveCursor(DIRECTION_UP);
      break;

    case 's':
      moveCursor(DIRECTION_DOWN);
      break;

    case 'a':
      moveCursor(DIRECTION_LEFT);
      break;

    case 'd':
      moveCursor(DIRECTION_RIGHT);
      break;

    case 'c':
      handler.clear();
      break;

    case 'r':
      handler.update();
      generation++;
      break;

    case 'p':
      autoRunning = !autoRunning;
      break;

    case '+':
      if (delayMs > 50) {
        delayMs -= 50;
      }
      break;

    case '-':
      if (delayMs < 2000) {
        delayMs += 50;
      }
      break;
    }
  }
}

void TerminalEditor::run() {
  enableRawMode();

  render();

  while (running) {
    if (autoRunning) {
      if (!hasAliveCells()) {
        autoRunning = false;
        render();
        clearScreen();
        std::cout << "All cells died after " << generation << " generations.\n";
        std::cout << "Press any key to continue in editor mode...\n";
        char c;
        read(STDIN_FILENO, &c, 1);
        render();
        continue;
      }

      if (checkInput(delayMs)) {
        handleInput();
      } else {
        handler.update();
        generation++;
      }
      render();
    } else {
      handleInput();
      render();
    }
  }

  disableRawMode();
  clearScreen();
  std::cout << "Editor closed. Final generation: " << generation << "\n";
}

} // namespace life
