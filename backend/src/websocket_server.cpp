#include "websocket_server.hpp"
#include "websocket_session.hpp" // Include session class header

WebSocketServer::WebSocketServer(boost::asio::io_context& ioc, boost::asio::ip::tcp::endpoint endpoint)
    : acceptor_(ioc, endpoint) {}

void WebSocketServer::start() {
    do_accept();
}

void WebSocketServer::do_accept() {
    acceptor_.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
        on_accept(ec, std::move(socket));
    });
}

void WebSocketServer::on_accept(boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
    if (!ec) {
        std::make_shared<WebSocketSession>(std::move(socket))->start();
    }
    do_accept();
}

void WebSocketServer::add_client(std::shared_ptr<WebSocketSession> session) {
    std::lock_guard<std::mutex> lock(clients_mutex_);
    connected_clients_.insert(session);
}

void WebSocketServer::remove_client(std::shared_ptr<WebSocketSession> session) {
    std::lock_guard<std::mutex> lock(clients_mutex_);
    connected_clients_.erase(session);
}

void WebSocketServer::broadcast(const std::string& message) {
    std::lock_guard<std::mutex> lock(clients_mutex_);
    for (const auto& client : connected_clients_) {
        client->send(message); // Assuming `send` is a function in WebSocketSession
    }
}
