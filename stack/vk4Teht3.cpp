#include "stack.hpp"
#include <cstdlib>
#include <iostream>

void primeHomma(Stack &stack, uint32_t number) {
  StackEntry divisor = 2;
  StackEntry temp = number;

  while (temp > 1) {
    if (temp % divisor != 0) {
      divisor++;
      continue;
    }
    stack.push(divisor);
    temp /= divisor;
  }
}

void printPrimes(Stack &primes) {
  StackEntry currentTop;

  while (!primes.empty()) {
    primes.top(currentTop);
    primes.pop();
    std::cout << currentTop << " ";
  }
  std::cout << "\n";
}

int main() {
  uint32_t num;
  std::cout << "Anna kokonaisluku: ";
  std::cin >> num;

  if (num < 2) {
    std::cout << "ei jakajia. Huono luku\n";
    return EXIT_FAILURE;
  }

  Stack primes;
  primeHomma(primes, num);
  printPrimes(primes);

  return EXIT_SUCCESS;
}
