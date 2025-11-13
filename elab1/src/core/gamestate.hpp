#pragma once

#include "sequence.hpp"

enum GameState {
  INITIAL,
  SLEEP,
  PLAYING,
  WIN,
  GAME_OVER,
};

GameState step(GameState current, Sequence *const sequence);
