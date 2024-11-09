#include "websocket_session.hpp"
#include "input_handler.hpp" // Include input handling functions
#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <nlohmann/json.hpp> // JSON library for parsing messages

using json = nlohmann::json;

WebSocketSession::WebSocketSession(boost::asio::ip::tcp::socket socket)
    : ws_(std::move(socket)) {}

void WebSocketSession::start() {
    server_.add_client(shared_from_this()); // Register session with server
    ws_.async_accept([self = shared_from_this()](boost::system::error_code ec) {
        self->on_accept(ec);
    });
}

void WebSocketSession::on_accept(boost::system::error_code ec) {
    if (ec) {
        server_.remove_client(shared_from_this()); // Remove on error
        return;
    }
    do_read();
}

void WebSocketSession::do_read() {
    ws_.async_read(buffer_, [self = shared_from_this()](boost::system::error_code ec, std::size_t bytes_transferred) {
        if (ec == websocket::error::closed) {
            self->server_.remove_client(self); // Remove on disconnect
        } else {
            self->on_read(ec, bytes_transferred);
        }
    });
}

void WebSocketSession::on_read(boost::system::error_code ec, std::size_t bytes_transferred) {
    if (!ec) {
    boost::ignore_unused(bytes_transferred);

    if (ec == websocket::error::closed) {
        return; // Client disconnected
    }

    if (ec) {
        std::cerr << "Read Error: " << ec.message() << "\n";
        return;
    }

    // Parse JSON message from buffer
    auto message = beast::buffers_to_string(buffer_.data());
    buffer_.consume(buffer_.size()); // Clear the buffer after reading

    try {
        json input = json::parse(message);
        std::string action = input["action"];
        float direction = input["direction"];

        if (action == "move") {
            handle_move_command(player_id_, direction); // Process move command
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << "\n";
    }


        do_read(); // Continue reading
    }
}

void WebSocketSession::on_write(boost::system::error_code ec, std::size_t bytes_transferred) {
    if (!ec) {
        // Ready for next message
    }
}

void WebSocketSession::send(const std::string& message) {
    ws_.async_write(boost::asio::buffer(message), [self = shared_from_this()](boost::system::error_code ec, std::size_t) {
        if (ec) {
            self->server_.remove_client(self); // Remove on write error
        }
    });
}
