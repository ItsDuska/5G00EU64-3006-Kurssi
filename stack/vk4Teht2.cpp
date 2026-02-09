#include "stack.hpp"
#include <iostream>

/* Tehtävän konsolin tulos. HUOM. deleteAll testi printtaa ne väärin päin, mutta
 itse funktio toimii :p
 * full() func:
 stack täynnä: 0

 popTop() func:
 popattu arvo: 5

 clear() func
 Stack tyhjä: 1

 deleteAll() func:
  Poistetaan luku 3
 stack poiston jälkeen: 5 4 2 1

 size() func:
 stack koko: 4
 */

namespace fakeStack {

bool full(Stack &stack) {
  StackEntry temp;
  if (stack.push(temp) == STACK_OVERFLOW) {
    return true;
  }
  stack.pop();
  return false;
}

ErrorCode popTop(Stack &stack, StackEntry &entry) {
  if (stack.empty())
    return STACK_UNDERFLOW;

  stack.top(entry);
  stack.pop();
  return STACK_SUCCESS;
}

void clear(Stack &stack) {
  StackEntry temp;
  while (!stack.empty()) {
    stack.pop(); // poistaa päällimmäisen
  }
}

int size(Stack &stack) {
  Stack temp;
  StackEntry currentTop;
  uint32_t count = 0;

  while (!stack.empty()) {
    stack.top(currentTop);
    stack.pop();
    temp.push(currentTop);
    ++count;
  }

  while (!temp.empty()) {
    temp.top(currentTop);
    temp.pop();
    stack.push(currentTop);
  }

  return count;
}

void deleteAll(Stack &stack, StackEntry entry) {
  Stack temp;
  StackEntry currentTop;

  while (!stack.empty()) {
    stack.top(currentTop);
    stack.pop();
    if (currentTop != entry)
      temp.push(currentTop);
  }

  while (!temp.empty()) {
    temp.top(currentTop);
    temp.pop();
    stack.push(currentTop);
  }
}

} // namespace fakeStack

int main() {
  Stack stack;
  StackEntry entry;

  for (int i = 1; i <= 5; ++i) {
    if (stack.push(i) != STACK_SUCCESS) {
      std::cout << "ERROR " << i << "\n";
      return EXIT_FAILURE;
    }
  }

  std::cout << "full() func:\n";
  bool isFull = fakeStack::full(stack);
  std::cout << "stack täynnä: " << isFull
            << "\n\n"; // en jaksa muuttaa bool arvoa kyllä tai ei stringiksi :p
                       // jotenka 1 ja 0 riittää.

  std::cout << "popTop() func:\n";
  if (fakeStack::popTop(stack, entry) == STACK_SUCCESS) {
    std::cout << "popattu arvo: " << entry << "\n\n";
  } else {
    std::cout << "popTop ERROR\n\n";
  }

  std::cout << "clear() func\n";
  fakeStack::clear(stack);
  std::cout << "Stack tyhjä: " << stack.empty() << "\n\n";

  std::cout << "deleteAll() func:\n Poistetaan luku 3\n";
  for (int i = 1; i <= 5; ++i) {
    stack.push(i);
  }

  fakeStack::deleteAll(stack, 3);

  std::cout << "stack poiston jälkeen: ";
  Stack temp = stack;
  while (!temp.empty()) {
    temp.top(entry);
    temp.pop();
    std::cout << entry << " ";
  }
  std::cout << "\n\n";

  std::cout << "size() func:\n";
  std::cout << "stack koko: " << fakeStack::size(stack) << "\n\n";
}
