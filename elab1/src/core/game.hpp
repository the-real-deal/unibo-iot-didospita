#pragma once

#include "../lib/timer.hpp"
#include "sequence.hpp"
#include <LiquidCrystal_I2C.h>

enum GameState {
  INITIAL,
  MENU,
  SHOW_DIFFICULTY,
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
  size_t difficulty;
  Sequence sequence;
  uint8_t score;
  Timer timer;
};

Game initGame();
void gameStep(Game *const game, LiquidCrystal_I2C *const lcd);
