#include "stack.hpp"
#include <iostream>

int main() {
  Stack source;
  Stack dest1, dest2, dest3;

  for (int i = 1; i <= 5; ++i) {
    if (source.push(i) != STACK_SUCCESS) {
      std::cout << "ERROR " << i << " \n";
      return EXIT_FAILURE;
    }
  }

  // copyStack1 (memcpy)
  if (source.copyStack1(dest1, source) == STACK_SUCCESS) {
    std::cout << "copyStack1 onnistui. dest1 sisältö: ";
    for (int i = 0; i < dest1.size(); ++i) {
      StackEntry x;
      dest1.top(x);
      std::cout << x << " ";
      dest1.pop();
    }
    std::cout << "\n";
  }

  // copyStack2 (temp stack)
  if (source.copyStack2(dest2, source) == STACK_SUCCESS) {
    std::cout << "copyStack2 onnistui. dest2 sisältö: ";
    for (int i = 0; i < dest2.size(); ++i) {
      StackEntry x;
      dest2.top(x);
      std::cout << x << " ";
      dest2.pop();
    }
    std::cout << "\n";
  }

  // copyStack3 (friend func)
  if (copyStack3(dest3, source) == STACK_SUCCESS) {
    std::cout << "copyStack3 onnistui. dest3 sisältö: ";
    for (int i = 0; i < dest3.size(); ++i) {
      StackEntry x;
      dest3.top(x);
      std::cout << x << " ";
      dest3.pop();
    }
    std::cout << "\n";
  }

  return 0;
}
