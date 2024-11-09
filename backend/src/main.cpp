#include <boost/asio.hpp>
#include "websocket_server.hpp"
#include "game_logic.hpp"
#include "game_state.hpp"
#include <iostream>
#include <bits/this_thread_sleep.h>
#include <nlohmann/json.hpp> // JSON library for parsing messages

using json = nlohmann::json;

// Port number for the WebSocket server
const int PORT = 8080;

void broadcast_state(const GameState& state, WebSocketServer& server) {
    json state_json = {
        {"player1_pos", state.player1_pos},
        {"player2_pos", state.player2_pos},
        {"ball_x", state.ball_x},
        {"ball_y", state.ball_y},
        {"player1_score", state.player1_score},
        {"player2_score", state.player2_score}
    };
    std::string message = state_json.dump();
    server.broadcast(message); // Send to all connected clients
}

int main() {
    try {
        // Set up the Boost ASIO context for asynchronous operations
        boost::asio::io_context ioc;

        // Define the server endpoint (e.g., listening on port 8080)
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), PORT);

        // Create and start the WebSocket server
        WebSocketServer server(ioc, endpoint);
        server.start();

        // Initialize the game state
        GameState game_state;
        initialize_game(game_state);

        // Run the main game loop
        std::thread game_loop_thread([&]() {
            while (true) {
                update_game(game_state);  // Update game logic (e.g., ball movement, collisions)
                
                // Broadcast the current game state to all connected clients
                broadcast_state(game_state);

                // Check for a winning condition
                if (check_win_condition(game_state)) {
                    broadcast_win_message(game_state);
                    initialize_game(game_state);  // Reset the game state
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Run at ~60 FPS
            }
        });

        // Run the ASIO I/O context to handle incoming connections
        ioc.run();

        // Join the game loop thread to ensure it completes before exiting
        game_loop_thread.join();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
