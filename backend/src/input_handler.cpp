#include <algorithm>

#include "input_handler.hpp"

void move_paddle(float& paddle_pos, float direction) {
    paddle_pos += direction * PADDLE_SPEED;
    paddle_pos = std::max(0.0f, std::min(1.0f, paddle_pos)); // Keep paddle within bounds
}

void handle_move_command(int player_id, float direction, GameState& state) {
    if (player_id == 1) {
        state.player1_pos = std::max(0.0f, std::min(1.0f, state.player1_pos + direction * PADDLE_SPEED));
    } else if (player_id == 2) {
        state.player2_pos = std::max(0.0f, std::min(1.0f, state.player2_pos + direction * PADDLE_SPEED));
    }
}
