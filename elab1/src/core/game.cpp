#include "game.hpp"
#include "../config.hpp"
#include "../lib/timer.hpp"
#include "../lib/utils.hpp"

Game initGame() {
  return {
      .state = INITIAL,
  };
}

void gameStep(Game *const game) {
  switch (game->state) {
  case INITIAL:
    game->timer = initTimer(IDLE_PERIOD_MS);
    Serial.println("Started idle wait timer: " + String((int)IDLE_PERIOD_MS) +
                   "ms");
    game->state = IDLE_WAIT;
    break;
  case IDLE_WAIT:
    Serial.println("Idle wait check");
    if (timerEnded(&game->timer)) {
      Serial.println("Idle finished");
      game->state = SLEEP;
    }
    break;
  case SLEEP:
    Serial.println("Starting deep sleeping");
    Serial.flush();
    deepSleep();
    break;
  case PLAYING:
    generateSequence(&game->sequence);
    if (game->sequence.status == COMPLETE) {
      game->state = WIN;
    } else if (game->sequence.status == FAIL) {
      game->state = GAME_OVER;
    } else {
      game->state = PLAYING;
    }
    break;
  case WIN:
    game->state = INITIAL;
    break;
  case GAME_OVER:
    game->state = INITIAL;
    break;
  default:
    game->state = INITIAL;
    break;
  }
}