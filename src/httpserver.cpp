//
// Copyright (c) 2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: HTTP server, small
//
//------------------------------------------------------------------------------

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include "bfb_algorithm.hpp"

namespace ip = boost::asio::ip;         // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

//namespace my_program_state
//{
//    std::size_t
//    request_count()
//    {
//        static std::size_t count = 0;
//        return ++count;
//    }
//
//    std::time_t
//    now()
//    {
//        return std::time(0);
//    }
//}

class http_connection : public std::enable_shared_from_this<http_connection>
{
public:
    http_connection(tcp::socket socket)
            : socket_(std::move(socket))
    {
    }

    // Initiate the asynchronous operations associated with the connection.
    void
    start()
    {
        read_request();
        check_deadline();
    }

private:
    // The socket for the currently connected client.
    tcp::socket socket_;

    // The buffer for performing reads.
    boost::beast::flat_buffer buffer_{8192};

    // The request message.
    http::request<http::dynamic_body> request_;

    // The response message.
    http::response<http::dynamic_body> response_;

    // The timer for putting a deadline on connection processing.
    boost::asio::basic_waitable_timer<std::chrono::steady_clock> deadline_{
            socket_.get_executor().context(), std::chrono::seconds(60)};

    // Asynchronously receive a complete request message.
    void
    read_request()
    {
        auto self = shared_from_this();

        http::async_read(
                socket_,
                buffer_,
                request_,
                [self](boost::beast::error_code ec,
                       std::size_t bytes_transferred)
                {
                    boost::ignore_unused(bytes_transferred);
                    if(!ec)
                        self->process_request();
                });
    }

    // Determine what needs to be done with the request message.
    void
    process_request()
    {
        response_.version(request_.version());
        response_.keep_alive(false);

        switch(request_.method())
        {
            case http::verb::get:
                response_.result(http::status::ok);
                response_.set(http::field::server, "Beast");
                create_response();
                break;

            default:
                // We return responses indicating an error if
                // we do not recognize the request method.
                response_.result(http::status::bad_request);
                response_.set(http::field::content_type, "text/plain");
                boost::beast::ostream(response_.body())
                        << "Invalid request-method '"
                        << request_.method_string().to_string()
                        << "'";
                break;
        }

        write_response();
    }

    // Construct a response message based on the program state.
    void
    create_response()
    {
        if(request_.target() == "/bfbSearch")
        {
            Graph g = Graph();
            g.addSegment(1,3,1);
            g.addSegment(2,4,1);
            g.addSegment(3,6,1);
            g.addJunction(1,'+',2,'+',3,1);
            g.addJunction(2,'+',3,'+',4,1);
            g.addJunction(3,'+',3,'-',3,1);
            g.addJunction(3,'-',3,'+',1,1);
            g.addJunction(1,'-',1,'+',1,1);
            BFBAlgorithm bfbAlgo = BFBAlgorithm(g);
            string result;
            if (bfbAlgo.BFBTraverseUtil()) {
                result = bfbAlgo.getResult();
            }
            response_.set(http::field::content_type, "text/html");
            boost::beast::ostream(response_.body())
                    << "<html>\n"
                    <<  "<head><title>A Possible Result</title></head>\n"
                    <<  "<body>\n"
                    <<  "<h1>Request count</h1>\n"
                    <<  "<p>"
                    <<  result
                    <<  "</p>\n"
                    <<  "</body>\n"
                    <<  "</html>\n";
        }
        else
        {
            response_.result(http::status::not_found);
            response_.set(http::field::content_type, "text/plain");
            boost::beast::ostream(response_.body()) << "File not found\r\n";
        }
    }

    // Asynchronously transmit the response message.
    void
    write_response()
    {
        auto self = shared_from_this();

        response_.set(http::field::content_length, response_.body().size());

        http::async_write(
                socket_,
                response_,
                [self](boost::beast::error_code ec, std::size_t)
                {
                    self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                    self->deadline_.cancel();
                });
    }

    // Check whether we have spent enough time on this connection.
    void
    check_deadline()
    {
        auto self = shared_from_this();

        deadline_.async_wait(
                [self](boost::beast::error_code ec)
                {
                    if(!ec)
                    {
                        // Close socket to cancel any outstanding operation.
                        self->socket_.close(ec);
                    }
                });
    }
};

// "Loop" forever accepting new connections.
class httpServer {
    void http_server(tcp::acceptor &acceptor, tcp::socket &socket) {
        acceptor.async_accept(socket,
                              [&](boost::beast::error_code ec) {
                                  if (!ec)
                                      std::make_shared<http_connection>(std::move(socket))->start();
                                  http_server(acceptor, socket);
                              });
    }
public:
    int run_server(char *_address, char *_port) {
        try {

            auto const address = boost::asio::ip::make_address(_address);
            unsigned short port = static_cast<unsigned short>(std::atoi(_port));

            boost::asio::io_context ioc{1};

            tcp::acceptor acceptor{ioc, {address, port}};
            tcp::socket socket{ioc};
            http_server(acceptor, socket);

            ioc.run();
        }
        catch (std::exception const &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }
};