#include "gamestate.hpp"

GameState step(const GameState current, Sequence *const sequence) {
  switch (current) {
  case INITIAL:
    generateSequence(sequence);
    return PLAYING;
  case SLEEP:
    // TODO: timer
    return INITIAL;
  case PLAYING:
    if (sequence->status == COMPLETE) {
      return WIN;
    } else if (sequence->status == FAIL) {
      return GAME_OVER;
    } else {
      return PLAYING;
    }
  case WIN:
    return INITIAL;
  case GAME_OVER:
    return INITIAL;
  default:
    return INITIAL;
  }
}