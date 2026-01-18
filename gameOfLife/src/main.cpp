#include "engine/CellHandler.hpp"
#include "parser/ArgumentParser.hpp"
#include "terminal/TerminalEditor.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  life::ArgumentParser parser(argc, argv);
  life::CellHandler handler(parser.getGridSize());

  life::TerminalEditor editor(handler);
  editor.run();

  return 0;
}
