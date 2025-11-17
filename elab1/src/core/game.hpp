#pragma once

#include "../lib/timer.hpp"
#include "sequence.hpp"

enum GameState {
  INITIAL,
  IDLE_WAIT,
  SLEEP,
  PLAYING,
  WIN,
  GAME_OVER,
};

struct Game {
  GameState state;
  Sequence sequence;
  Timer timer;
};

Game initGame();
void gameStep(Game *const game);
