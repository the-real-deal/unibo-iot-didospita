#include "game.hpp"
#include "../config.hpp"
#include "../lib/led.hpp"
#include "../lib/potentiometer.hpp"
#include "../lib/timer.hpp"
#include "../lib/utils.hpp"
#include "led.hpp"

Game initGame() {
  return Game{
      .state = INITIAL,
      .difficulty = 0,
  };
}

void gameStep(Game *const game, LiquidCrystal_I2C *const lcd) {
  static int32_t controlLedFade;
  const size_t difficulty =
      readPotentiometer(POTENTIOMETER_PIN, N_DIFFICULTIES);
  const bool isTimerEnded = timerEnded(&game->timer);

  switch (game->state) {
  case INITIAL:
    controlLedFade = 0;
    game->timer = initTimer(IDLE_PERIOD_MS);
    lcd->clear();
    game->state = MENU;
    break;
  case MENU:
    if (difficulty != game->difficulty) {
      game->difficulty = difficulty;
      game->timer = initTimer(DIFFICULTY_SHOW_PERIOD_MS);
      lcd->clear();
      game->state = SHOW_DIFFICULTY;
    } else {
      lcd->setCursor(0, 0);
      lcd->print("Welcome to TOS!");
      lcd->setCursor(0, 1);
      lcd->print("Press B1 to start");
      lcd->flush();
    }
    if (isTimerEnded) {
      game->state = SLEEP;
    }
    fadeLed(CONTROL_LED_PIN, &controlLedFade);
    break;
  case SHOW_DIFFICULTY:
    lcd->setCursor(0, 0);
    lcd->print("Difficulty: " + String(game->difficulty + 1));
    lcd->flush();
    if (isTimerEnded) {
      game->state = INITIAL;
    }
    fadeLed(CONTROL_LED_PIN, &controlLedFade);
    break;
  case SLEEP:
    turnOffLed(CONTROL_LED_PIN);
    turnOffAllGameLeds();
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Press B1 to");
    lcd->setCursor(0, 1);
    lcd->print("wake up");
    lcd->flush();
    deepSleep();
    break;
  case INIT_GAME:
    turnOffLed(CONTROL_LED_PIN);
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("GO!");
    lcd->flush();
    game->timer = initTimer(INIT_GAME_PERIOD_MS);
    game->state = STARTING_GAME;
    break;
  case STARTING_GAME:
    if (timerEnded(&game->timer)) {
      game->score = 0;
      game->round = 20;
      game->state = INIT_ROUND;
    }
    break;
  case INIT_ROUND:
    turnOffLed(CONTROL_LED_PIN);
    turnOffAllGameLeds();
    generateSequence(&game->sequence);
    game->timer = initTimer(
        max(ROUND_PERIOD_MS - ((int64_t)DIFFICULTY_FACTORS[game->difficulty] *
                               (int64_t)game->round),
            (int64_t)ROUND_TIME_LBOUND_MS[game->difficulty]));
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Round: " + String(game->round + 1));
    lcd->setCursor(0, 1);
    lcd->print(getSequenceString(&game->sequence));
    lcd->flush();
    game->state = PLAYING;
    break;
  case PLAYING:
    if (isTimerEnded) {
      turnOnLed(CONTROL_LED_PIN);
      turnOffAllGameLeds();
      game->timer = initTimer(GAME_OVER_LED_PERIOD_MS);
      game->state = INIT_GAME_OVER;
    } else {
      switch (game->sequence.status) {
      case COMPLETE:
        turnOnAllGameLeds();
        game->score += WIN_POINTS;
        game->round++;
        game->timer = initTimer(WIN_PERIOD_MS);
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print("GOOD! Score: " + String(game->score));
        lcd->flush();
        game->state = WIN;
        break;
      case FAIL:
        turnOnLed(CONTROL_LED_PIN);
        turnOffAllGameLeds();
        game->timer = initTimer(GAME_OVER_LED_PERIOD_MS);
        game->state = INIT_GAME_OVER;
        break;
      default:
        break;
      }
    }
    break;
  case WIN:
    if (isTimerEnded) {
      game->state = INIT_ROUND;
    }
    break;
  case INIT_GAME_OVER:
    if (isTimerEnded) {
      lcd->clear();
      lcd->setCursor(0, 0);
      lcd->print("Game Over");
      lcd->setCursor(0, 1);
      lcd->print("Score: " + String(game->score));
      lcd->flush();
      turnOffLed(CONTROL_LED_PIN);
      game->timer = initTimer(GAME_OVER_PERIOD_MS);
      game->state = GAME_OVER;
    }
    break;
  case GAME_OVER:
    if (isTimerEnded) {
      game->state = INITIAL;
    }
    break;
  }
}
