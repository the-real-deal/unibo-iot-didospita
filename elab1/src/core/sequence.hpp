#pragma once

#include "config.hpp"
#include <stdint.h>
#include <stdlib.h>

typedef uint8_t sequenceVal_t;

enum SequenceStatus {
  CONTINUE,
  COMPLETE,
  FAIL,
};

struct Sequence {
  sequenceVal_t values[SEQUENCE_LENGTH];
  size_t index;
  SequenceStatus status;
};

void generateSequence(Sequence *const sequence);
void advanceSequence(Sequence *const sequence, const sequenceVal_t value);
String getSequence(Sequence* const sequence);
