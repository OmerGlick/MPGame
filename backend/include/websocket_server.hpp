#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <unordered_set>
#include <mutex> // For thread-safe access
#include "websocket_session.hpp"

class WebSocketServer {
public:
    WebSocketServer(boost::asio::io_context& ioc, boost::asio::ip::tcp::endpoint endpoint);
    void start();

    // Add or remove clients from connected_clients
    void add_client(std::shared_ptr<WebSocketSession> session);
    void remove_client(std::shared_ptr<WebSocketSession> session);

    // Broadcast function to send messages to all connected clients
    void broadcast(const std::string& message);
    
private:
    void do_accept();
    void on_accept(boost::system::error_code ec, boost::asio::ip::tcp::socket socket);

    boost::asio::ip::tcp::acceptor acceptor_;

    // Container to store active sessions
    std::unordered_set<std::shared_ptr<WebSocketSession>> connected_clients_;
    std::mutex clients_mutex_; // Mutex to ensure thread-safe access
};
