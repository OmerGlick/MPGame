#pragma once

#include "game_state.hpp"

void update_ball(GameState& state);
void update_game(GameState& state);
bool check_win_condition(const GameState& state);
void check_score(GameState& state);
