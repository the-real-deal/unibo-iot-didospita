#include "sequence.hpp"

void generateSequence(int *const seq, const size_t length) {
  bool gen[length + 1] = {0};
  for (size_t i = 0; i < length; i++) {
    int val;
    do {
      val = (rand() % length) + 1;
    } while (gen[val]);
    gen[val] = true;
    seq[i] = val;
  }
}