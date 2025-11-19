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
  size_t difficulty;
  switch (game->state) {
  case INITIAL:
    controlLedFade = 0;
    game->timer = initTimer(IDLE_PERIOD_MS);
    lcd->clear();
    game->state = MENU;
    break;
  case MENU:
    difficulty = readPotentiometer(POTENTIOMETER_PIN, N_DIFFICULTIES);

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
    if (timerEnded(&game->timer)) {
      game->state = SLEEP;
    }
    fadeLed(CONTROL_LED_PIN, &controlLedFade);
    break;
  case SHOW_DIFFICULTY:
    lcd->setCursor(0, 0);
    lcd->print("Difficulty: " + String(game->difficulty + 1));
    lcd->flush();
    if (timerEnded(&game->timer)) {
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
      game->round = 0;
      game->state = INIT_ROUND;
    }
    break;
  case INIT_ROUND:
    turnOffLed(CONTROL_LED_PIN);
    turnOffAllGameLeds();
    generateSequence(&game->sequence);
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Round: " + String(game->round + 1));
    lcd->setCursor(0, 1);
    lcd->print(getSequence(&game->sequence));
    lcd->flush();
    game->timer = initTimer(max(
        ROUND_PERIOD_MS - (DIFFICULTY_FACTORS[game->difficulty] * game->round),
        ROUND_TIME_LBOUND_MS[game->difficulty]));
    Serial.println((int32_t)game->timer.period);
    Serial.flush();
    game->state = PLAYING;
    break;
  case PLAYING:
    if (timerEnded(&game->timer)) {
      game->state = INIT_GAME_OVER;
      turnOnLed(CONTROL_LED_PIN);
      turnOffAllGameLeds();
      game->timer = initTimer(GAME_OVER_LED_PERIOD_MS);
    } else {
      if (game->sequence.status == COMPLETE) {
        game->score += WIN_POINTS;
        game->round++;
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print("GOOD! Score: " + String(game->score));
        lcd->flush();
        game->timer = initTimer(WIN_PERIOD_MS);
        turnOnAllGameLeds();
        game->state = WIN;
      } else if (game->sequence.status == FAIL) {
        turnOnLed(CONTROL_LED_PIN);
        turnOffAllGameLeds();
        game->timer = initTimer(GAME_OVER_LED_PERIOD_MS);
        game->state = INIT_GAME_OVER;
      } else {
        game->state = PLAYING;
      }
    }
    break;
  case WIN:
    if (timerEnded(&game->timer)) {
      game->state = INIT_ROUND;
    }
    break;
  case INIT_GAME_OVER:
    if (timerEnded(&game->timer)) {
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
    if (timerEnded(&game->timer)) {
      game->state = INITIAL;
    }
    break;
  }
}
