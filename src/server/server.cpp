//
//  mbus.cpp
//  mbus
//
//  Created by davidqian on 16/11/29.
//  Copyright (c) 2016年 davidqian. All rights reserved.
//

#include "server.hpp"
#include <signal.h>
#include <utility>

namespace mbus {

        server::server(const std::string& address, const std::string& port)
                : io_service_(),
                  signals_(io_service_),
                  acceptor_(io_service_),
                  connection_manager_(),
                  socket_(io_service_),
                  timer_(io_service_, boost::posix_time::seconds(1))
        {
            // Register to handle the signals that indicate when the server should exit.
            // It is safe to register for the same signal multiple times in a program,
            // provided all registration for the specified signal is made through Asio.
            signals_.add(SIGINT);
            signals_.add(SIGTERM);
#if defined(SIGQUIT)
            signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)

            do_await_stop();

            // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
            boost::asio::ip::tcp::resolver resolver(io_service_);
            boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
            acceptor_.open(endpoint.protocol());
            acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
            acceptor_.bind(endpoint);
            acceptor_.listen();

            do_accept();

            timer_.async_wait(boost::bind(&server::heart_beat_timer, this));
        }

        void server::run()
        {
	    auto msgThread = new std::thread(server::consume_msg_queue_thread, this);
            msgThread->detach();

            // The io_service::run() call will block until all asynchronous operations
            // have finished. While the server is running, there is always at least one
            // asynchronous operation outstanding: the asynchronous accept call waiting
            // for new incoming connections.
            io_service_.run();
        }

        void server::do_accept()
        {
            acceptor_.async_accept(socket_,
                                   [this](boost::system::error_code ec)
                                   {
					std::cout << "receive one connector" << std::endl;
                                       // Check whether the server was stopped by a signal before this
                                       // completion handler had a chance to run.
                                       if (!acceptor_.is_open())
                                       {
                                           return;
                                       }

                                       if (!ec)
                                       {
                                           connection_manager_.add(std::make_shared<connection>(
                                                   std::move(socket_), connection_manager_));
                                       }

                                       do_accept();
                                   });
        }

        void server::do_await_stop()
        {
            signals_.async_wait(
                    [this](boost::system::error_code /*ec*/, int /*signo*/)
                    {
                        // The server is stopped by cancelling all outstanding asynchronous
                        // operations. Once all operations have finished the io_service::run()
                        // call will exit.
                        acceptor_.close();
                        connection_manager_.stop_all();
                    });
        }

        void server::heart_beat_timer()
        {
            connection_manager_.check_heart_beat();
            timer_.expires_from_now(boost::posix_time::seconds(1));
            timer_.async_wait(boost::bind(&server::heart_beat_timer, this));
        }

        void server::consume_msg_queue_thread(server *srv) {
            while(true){
                srv->connection_manager_.consume_msg();
            }
        }

} // namespace mbus
