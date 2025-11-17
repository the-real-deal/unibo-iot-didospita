#include "game.hpp"
#include "../config.hpp"
#include "../lib/timer.hpp"
#include "../lib/utils.hpp"
#include "../lib/led.hpp"

Game initGame() {
  return {
      .state = INITIAL,
  };
}

void gameStep(Game *const game) {
  static int32_t controlLedFade;
  switch (game->state) {
  case INITIAL:
    controlLedFade = 0;
    game->timer = initTimer(IDLE_PERIOD_MS);
    Serial.println("Started idle wait timer: " + String((int)IDLE_PERIOD_MS) +
                   "ms");
    game->state = IDLE_WAIT;
    break;
  case IDLE_WAIT:
    Serial.println("Idle wait check");
    fadeLed(CONTROL_LED_PIN, &controlLedFade);
    Serial.println(controlLedFade);
    Serial.flush();
    if (timerEnded(&game->timer)) {
      Serial.println("Idle finished");
      game->state = SLEEP;
    }
    break;
  case SLEEP:
    Serial.println("Starting deep sleeping");
    Serial.flush();
    turnOffLed(CONTROL_LED_PIN);
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