//
//  server.hpp
//  mbus
//
//  Created by davidqian on 16/11/29.
//  Copyright (c) 2016年 davidqian. All rights reserved.
//

#ifndef MBUS_SERVER_HPP
#define MBUS_SERVER_HPP

#include <string>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "connection/connection.hpp"
#include "connection/connection_manager.hpp"

namespace mbus {

/// The top-level class of the mbus server.
        class server
        {
        public:
            server(const server&) = delete;
            server& operator=(const server&) = delete;

            /// Construct the server to listen on the specified TCP address and port, and
            /// serve up files from the given directory.
            explicit server(const std::string& address, const std::string& port);

            /// Run the server's io_service loop.
            void run();

            void heart_beat_timer();

            void consume_msg_queue_thread(server * srv);

            boost::asio::deadline_timer timer_;

        private:
            /// Perform an asynchronous accept operation.
            void do_accept();

            /// Wait for a request to stop the server.
            void do_await_stop();

            /// The io_service used to perform asynchronous operations.
            boost::asio::io_service io_service_;

            /// The signal_set is used to register for process termination notifications.
            boost::asio::signal_set signals_;

            /// Acceptor used to listen for incoming connections.
            boost::asio::ip::tcp::acceptor acceptor_;

            /// The connection manager which owns all live connections.
            connection_manager connection_manager_;

            /// The next socket to be accepted.
            boost::asio::ip::tcp::socket socket_;
        };

} // namespace http

#endif // MBUS_SERVER_HPP