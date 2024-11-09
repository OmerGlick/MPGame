#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <thread>
#include <unordered_map>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;   // from <boost/asio/ip/tcp.hpp>

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
    explicit WebSocketSession(tcp::socket socket)
        : ws_(std::move(socket)) {}

    // Start the session
    void start() {
        // Accept the WebSocket connection using a lambda function
        ws_.async_accept([self = shared_from_this()](beast::error_code ec) {
            self->on_accept(ec);
        });
    }

private:
    websocket::stream<tcp::socket> ws_;

    // Handle accepting the WebSocket connection
    void on_accept(beast::error_code ec) {
        if (ec) {
            std::cerr << "Accept Error: " << ec.message() << "\n";
            return;
        }

        // Read messages from the WebSocket client
        do_read();
    }

    // Read message from client
    void do_read() {
        ws_.async_read(buffer_, [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
            self->on_read(ec, bytes_transferred);
        });
    }

    // Handle received message
    void on_read(beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);

        if (ec == websocket::error::closed) {
            return; // Connection closed by client
        }

        if (ec) {
            std::cerr << "Read Error: " << ec.message() << "\n";
            return;
        }

        // Process the received message
        auto message = beast::buffers_to_string(buffer_.data());
        std::cout << "Received Message: " << message << "\n";

        // Clear buffer after reading
        buffer_.consume(buffer_.size());

        // Echo back message to client (for testing)
        ws_.text(ws_.got_text());
        ws_.async_write(net::buffer(message), [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
            self->on_write(ec, bytes_transferred);
        });

        // Keep reading from client
        do_read();
    }

    // Handle writing message back to client
    void on_write(beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);

        if (ec) {
            std::cerr << "Write Error: " << ec.message() << "\n";
            return;
        }
    }

    beast::flat_buffer buffer_;
};

class WebSocketServer {
public:
    WebSocketServer(net::io_context& ioc, tcp::endpoint endpoint)
        : acceptor_(ioc, endpoint) {
        do_accept();
    }

private:
    tcp::acceptor acceptor_;

    void do_accept() {
        acceptor_.async_accept([this](beast::error_code ec, tcp::socket socket) {
            on_accept(ec, std::move(socket));
        });
    }

    void on_accept(beast::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::make_shared<WebSocketSession>(std::move(socket))->start();
        } else {
            std::cerr << "Accept Error: " << ec.message() << "\n";
        }

        // Accept next connection
        do_accept();
    }
};

int main() {
    try {
        // Create the I/O context and listen on port 8080
        net::io_context ioc;
        tcp::endpoint endpoint{tcp::v4(), 8080};

        WebSocketServer server(ioc, endpoint);

        // Run the I/O context
        ioc.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
