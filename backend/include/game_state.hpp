#pragma once

// Constants
const int WINNING_SCORE = 5;
const int PADDLE_HEIGHT = 4; // Height in units
const int PADDLE_WIDTH = 1;
const int BALL_SIZE = 1;
const float PADDLE_SPEED = 0.5f; // Speed of paddle movement
const float BALL_SPEED = 0.75f;   // Speed of ball movement

struct GameState {
    float player1_pos;
    float player2_pos;
    float ball_x, ball_y;
    float ball_vel_x, ball_vel_y;
    int player1_score, player2_score;
};

void initialize_game(GameState& state);
void reset_ball(GameState& state);
