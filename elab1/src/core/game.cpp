#include "game.hpp"
#include "../config.hpp"
#include "../lib/timer.hpp"
#include "../lib/utils.hpp"
#include "../lib/led.hpp"
#include "led.hpp"

Game initGame() {
  return {
      .state = INITIAL,
  };
}

void gameStep(Game *const game, LiquidCrystal_I2C* const lcd) {
  static int32_t controlLedFade;
  switch (game->state) {
    case INITIAL:
      controlLedFade = 0;
      game->score = 0;
      game->timer = initTimer(IDLE_PERIOD_MS);
          
      lcd->clear();
      lcd->setCursor(0,0);
      lcd->print("Welcome to TOS!");
      lcd->setCursor(0,1);
      lcd->print("Press B1 to Start");
      lcd->flush();
      game->state = IDLE_WAIT;
      break;
    case IDLE_WAIT:
      fadeLed(CONTROL_LED_PIN, &controlLedFade);
      if (timerEnded(&game->timer)) {
        game->state = SLEEP;
      } 
      break;
    case SLEEP:
      turnOffLed(CONTROL_LED_PIN);
      turnOffAllGameLeds();
      lcd->clear();
      lcd->setCursor(0,0);
      lcd->print("Press B1 to");
      lcd->setCursor(0,1);
      lcd->print("wake up");
      deepSleep();
      break;
    case INIT_GAME: 
      turnOffLed(CONTROL_LED_PIN);
      /*DA CAMBIARE FORSE*/
      lcd->clear();
      lcd->setCursor(0,0);
      lcd->print("GO!");
      lcd->flush();
      game->timer = initTimer(INIT_GAME_PERIOD_MS);
      game->state = GO_WAITING;
      break;
    case GO_WAITING:
      if(timerEnded(&game->timer)) {
        game->state = INIT_ROUND;
      }
      break;
    case INIT_ROUND:
      turnOffLed(CONTROL_LED_PIN);
      turnOffAllGameLeds();
      generateSequence(&game->sequence);
      lcd->clear();
      lcd->setCursor(0,0);
      lcd->print(getSequence(&game->sequence));
      lcd->flush();
      game->timer = initTimer(ROUND_PERIOD_MS);
      game->state = PLAYING;
      break;
    case PLAYING:
      if(timerEnded(&game->timer)){
        game->state = INIT_GAME_OVER;
        turnOnLed(CONTROL_LED_PIN);
        turnOffAllGameLeds();
        game->timer = initTimer(INIT_GAME_OVER_PERIOD_MS);
      }
      else {
        if (game->sequence.status == COMPLETE) {
          lcd->clear();
          lcd->setCursor(0,0);
          game->score += 1;
          lcd->print("GOOD! Score: " + String(game->score));
          lcd->flush();
          game->timer = initTimer(WIN_PERIOD_MS);
          turnOnAllGameLeds();
          game->state = WIN;
        } else if (game->sequence.status == FAIL) {
          turnOnLed(CONTROL_LED_PIN);
          turnOffAllGameLeds();
          game->timer = initTimer(INIT_GAME_OVER_PERIOD_MS);
          game->state = INIT_GAME_OVER;
        } else {
          game->state = PLAYING;
        }
      }
      break;
    case WIN:
      if (timerEnded(&game->timer))
      {
        game->state = INIT_ROUND; 
      }
      break;
    case INIT_GAME_OVER:
      if (timerEnded(&game->timer))
      {
        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("Game Over");
        lcd->setCursor(0,1);
        lcd->print("Score: " + String(game->score));
        lcd->flush();
        turnOffLed(CONTROL_LED_PIN);
        game->timer = initTimer(GAME_OVER_PERIOD_MS);
              game->state = GAME_OVER; 
      }
    case GAME_OVER:
      if(timerEnded(&game->timer)) {
        game->state = INITIAL;
      }
      break;
    default:
      game->state = INITIAL;
      break;
  }
}