#pragma once

#include "../lib/timer.hpp"
#include "sequence.hpp"
#include <LiquidCrystal_I2C.h>

enum GameState {
  INITIAL,
  IDLE_WAIT,
  SLEEP,
  INIT_GAME,
  GO_WAITING,
  INIT_ROUND,
  PLAYING,
  WIN,
  INIT_GAME_OVER,
  GAME_OVER,
};

struct Game {
  GameState state;
  Sequence sequence;
  uint8_t score;
  Timer timer;
};

Game initGame();
void gameStep(Game *const game, LiquidCrystal_I2C* const lcd);
