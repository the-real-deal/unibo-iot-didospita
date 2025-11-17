#pragma once

#include "../lib/timer.hpp"
#include "sequence.hpp"
#include <LiquidCrystal_I2C.h>

enum GameState {
  INITIAL,
  MENU,
  SLEEP,
  INIT_GAME,
  STARTING_GAME,
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
  size_t difficulty;
};

Game initGame();
void gameStep(Game *const game, LiquidCrystal_I2C *const lcd);
