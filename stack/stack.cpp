#include "stack.hpp"
#include <cstdlib>

/*
 * Vastaukset tehtävän lopussa oleviin kysymyksiin:
 *
 * Helpoin kirjoittaa oli memcpy, sillä se on vain yksi rivi.
 *
 * Jos pino on täynnä, niin nopein funktio on se memcpy. Friend funktio on myös
 * nopea, mutta compiler muuttaa sen luultavasti memcpyyn, joten ne ovat
 * periaatteessa sama funktio.
 *
 * Jos pino on lähes tyhjä, niin memcpy on luultavasti nopein, mutta myös friend
 * funktio on nopea.
 *
 * Jos toteutus muuttuu, niin temp stack voisi olla kai parhain? Tosin se
 * riippuu siitä miten se toteutus muuttuu.
 *
 * Ensimmäisessä ja viimeisessä toteutuksessa voi käyttää constia.
 */
Stack::Stack() : count(0) {}
ErrorCode copyStack3(Stack &dest, Stack &source) {
  if (source.count >= MAX_STACK) {
    return STACK_OVERFLOW;
  }

  for (int i = 0; i <= source.count; i++) {
    dest.entry[i] = source.entry[i];
  }

  dest.count = source.count;
  return STACK_SUCCESS;
}

ErrorCode Stack::push(const StackEntry &item) {
  if (count >= MAX_STACK) {
    return STACK_OVERFLOW;
  }

  entry[count++] = item;
  return STACK_SUCCESS;
}

ErrorCode Stack::pop() {
  if (count == 0) {
    return STACK_UNDERFLOW;
  }

  count--;
  return STACK_SUCCESS;
}

ErrorCode Stack::top(StackEntry &item) const {
  if (count == 0) {
    return STACK_UNDERFLOW;
  }

  const uint32_t index = count - 1;
  item = entry[index];
  return STACK_SUCCESS;
}

ErrorCode Stack::copyStack1(Stack &dest, Stack &source) {
  if (source.count >= MAX_STACK) {
    return STACK_OVERFLOW;
  }

  dest = source;
  return STACK_SUCCESS;
}

ErrorCode Stack::copyStack2(Stack &dest, Stack &source) {
  if (source.count >= MAX_STACK) {
    return STACK_OVERFLOW;
  }

  Stack temp;
  StackEntry x;

  while (!source.empty()) {
    source.top(x);
    source.pop();
    temp.push(x);
  }

  while (!temp.empty()) {
    temp.top(x);
    temp.pop();
    source.push(x);
    dest.push(x);
  }

  return STACK_SUCCESS;
}

bool Stack::empty() const { return count == 0; }

bool Stack::full() const { return count == MAX_STACK; }

void Stack::clear() { count = 0; }

uint32_t Stack::size() { return count; }
