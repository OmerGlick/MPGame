#include "game_state.hpp"
#include <cstdlib>

void initialize_game(GameState& state) {
    state.player1_pos = 0.5f;
    state.player2_pos = 0.5f;
    reset_ball(state);
    state.player1_score = 0;
    state.player2_score = 0;
}

void reset_ball(GameState& state) {
    state.ball_x = 0.5f;
    state.ball_y = 0.5f;
    state.ball_vel_x = BALL_SPEED * (((rand() % 2) == 0) ? 1 : -1);
    state.ball_vel_y = BALL_SPEED * (((rand() % 2) == 0) ? 1 : -1);
}
