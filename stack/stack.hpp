#include <cstring>
#include <stdint.h>

typedef int StackEntry;

enum ErrorCode : char { STACK_SUCCESS, STACK_OVERFLOW, STACK_UNDERFLOW };

constexpr uint32_t MAX_STACK = 10; // small value for testing
class Stack {
public:
  Stack();
  ErrorCode push(const StackEntry &item);
  ErrorCode pop();
  ErrorCode top(StackEntry &item) const;
  ErrorCode copyStack1(Stack &dest, Stack &source);
  ErrorCode copyStack2(Stack &dest, Stack &source);
  ErrorCode copyStack3(Stack &dest, Stack &source);

  bool empty() const;
  bool full() const;
  void clear();

  uint32_t size();

  Stack &operator=(const Stack &stack) {
    if (this == &stack) {
      return *this;
    }
    count = stack.count;

    std::memcpy(entry, stack.entry, (count + 1) * sizeof(uint32_t));
    return *this;
  }

  friend ErrorCode copyStack3(Stack &dest, Stack &source);

private:
  int count;
  StackEntry entry[MAX_STACK];
};
