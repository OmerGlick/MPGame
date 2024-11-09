#pragma once

#include "game_state.hpp"

void move_paddle(float& paddle_pos, float direction);
void handle_move_command(int player_id, float direction, GameState& state);
