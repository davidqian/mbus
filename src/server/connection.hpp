//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MBUS_CONNECTION_HPP
#define MBUS_CONNECTION_HPP

#include <array>
#include <memory>
#include <vector>
#include <boost/asio.hpp>
#include "message.hpp"
#include "message_handler.hpp"
#include "message_parser.hpp"
#include "../util/util.hpp"

namespace mbus {
    namespace server {

        class connection_manager;

/// Represents a single connection from a client.
        class connection
                : public std::enable_shared_from_this<connection>
        {
        public:
            connection(const connection&) = delete;
            connection& operator=(const connection&) = delete;

            /// Construct a connection with the given socket.
            explicit connection(boost::asio::ip::tcp::socket socket,
                                connection_manager& manager, message_handler& handler);

            /// Start the first asynchronous operation for the connection.
            void start();

            /// Stop all asynchronous operations associated with the connection.
            void stop();

	    int get_remote_ip();

        private:
            /// Perform an asynchronous read operation.
            void do_read();

            /// Perform an asynchronous write operation.
            void do_write(std::vector<boost::asio::const_buffer> buf);

            /// Socket for the connection.
            boost::asio::ip::tcp::socket socket_;

            /// The manager for this connection.
            connection_manager& connection_manager_;

            /// The handler used to process the incoming request.
            message_handler& message_handler_;

            /// Buffer for incoming data.
            std::array<char, 8192> buffer_;

            /// The incoming request.
            message msg_;

            /// The parser for the incoming request.
            message_parser message_parser_;
        };

        typedef std::shared_ptr<connection> connection_ptr;

    } // namespace server
} // namespace mbus

#endif // MBUS_CONNECTION_HPP
