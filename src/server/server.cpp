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
            signals_.add(SIGINT);
            signals_.add(SIGTERM);
            #if defined(SIGQUIT)
              signals_.add(SIGQUIT);
            #endif

            do_await_stop();

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

            io_service_.run();
        }

        void server::do_accept()
        {
            acceptor_.async_accept(socket_,
                                   [this](boost::system::error_code ec)
                                   {
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
                        acceptor_.close();
                        connection_manager_.stop_all();
			io_service_.stop();
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
