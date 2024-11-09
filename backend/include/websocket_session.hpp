#pragma once

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <memory>

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
    explicit WebSocketSession(boost::asio::ip::tcp::socket socket);
    void start();
    // Send message to this specific client
    void send(const std::string& message);

private:
    void on_accept(boost::system::error_code ec);
    void do_read();
    void on_read(boost::system::error_code ec, std::size_t bytes_transferred);
    void on_write(boost::system::error_code ec, std::size_t bytes_transferred);

    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;
    boost::beast::flat_buffer buffer_;
    WebSocketServer& server_; // Reference to WebSocketServer
};
