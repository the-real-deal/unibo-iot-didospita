#include "sequence.hpp"
#include <Arduino.h>
#include <assert.h>

void generateSequence(Sequence *const sequence) {
  bool gen[SEQUENCE_LENGTH + 1] = {0};
  for (size_t i = 0; i < SEQUENCE_LENGTH; i++) {
    int val;
    do {
      val = (rand() % SEQUENCE_LENGTH) + 1;
    } while (gen[val]);
    gen[val] = true;
    sequence->values[i] = val;
  }
  sequence->index = 0;
  sequence->status = CONTINUE;
}

void advanceSequence(Sequence *const sequence, const sequenceVal_t value) {
  assert(sequence->status == CONTINUE);
  assert(sequence->index >= 0 && sequence->index < SEQUENCE_LENGTH);
  const bool correct = sequence->values[sequence->index] == value;
  if (correct) {
    sequence->index++;
    if (sequence->index == SEQUENCE_LENGTH) {
      sequence->status = COMPLETE;
    }
    // else remain in CONTINUE
  } else {
    sequence->status = FAIL;
  }
}

String getSequenceString(Sequence *const sequence) {
  char seq[SEQUENCE_LENGTH + 1];
  for (size_t i = 0; i < SEQUENCE_LENGTH; i++) {
    /*
    sequence values are always single digit, so we can use buff[2] to store
    the digit and the null terminator
    */
    char buff[2];
    sprintf(buff, "%d", sequence->values[i]);
    seq[i] = buff[0];
  }
  seq[SEQUENCE_LENGTH] = '\0';
  return String(seq);
}
