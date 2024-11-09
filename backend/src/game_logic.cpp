#include <complex>
#include "game_logic.hpp"

void update_ball(GameState& state) {
    // Update ball position
    state.ball_x += state.ball_vel_x;
    state.ball_y += state.ball_vel_y;

    // Wall collision
    if (state.ball_y <= 0.0f || state.ball_y >= 1.0f) {
        state.ball_vel_y = -state.ball_vel_y; // Bounce off top/bottom
    }

    // Paddle collisions
    if (state.ball_x <= 0.0f && std::fabs(state.ball_y - state.player1_pos) < PADDLE_HEIGHT / 2.0f) {
        state.ball_vel_x = -state.ball_vel_x;
    } else if (state.ball_x >= 1.0f && std::fabs(state.ball_y - state.player2_pos) < PADDLE_HEIGHT / 2.0f) {
        state.ball_vel_x = -state.ball_vel_x;
    }
}

bool check_win_condition(const GameState& state) {
    return state.player1_score >= WINNING_SCORE || state.player2_score >= WINNING_SCORE;
}

void check_score(GameState& state) {
    if (state.ball_x <= 0.0f) {
        state.player2_score++;
        reset_ball(state); // Reset ball to center
    } else if (state.ball_x >= 1.0f) {
        state.player1_score++;
        reset_ball(state); // Reset ball to center
    }
}

void update_game(GameState& state) {
    update_ball(state);
    check_score(state);
}
