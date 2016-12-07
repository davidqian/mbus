//
// connection.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "connection.hpp"
#include <utility>
#include <vector>
#include "connection_manager.hpp"
#include "message_handler.hpp"
#include "../util/util.hpp";

namespace mbus {
    namespace server {

        connection::connection(boost::asio::ip::tcp::socket socket,
                               connection_manager& manager, message_handler& handler)
                : socket_(std::move(socket)),
                  connection_manager_(manager),
                  message_handler_(handler)
        {
        }

        void connection::start()
        {
            do_read();
        }

        void connection::stop()
        {
            socket_.close();
        }

        void connection::do_read()
        {
            auto self(shared_from_this());
            socket_.async_read_some(boost::asio::buffer(buffer_),
                                    [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
                                    {
                                        if (!ec)
                                        {
                                            message_parser::result_type result;
                                            std::tie(result, std::ignore) = message_parser_.parse(
                                                    msg_, buffer_.data(), buffer_.data() + bytes_transferred);

                                            if (result == message_parser::good)
                                            {
                                                message_parser_.state_ = message_parser_.header;
                                                connection_ptr to_conn;
                                                connection_manager_.find(msg_.ip, to_conn);
                                                if(to_conn != nullptr){
                                                    try{
                                                        int remote_ip = util::ip2long(socket_.remote_endpoint.address().to_string());
                                                        to_conn.do_write(toBuffer(msg_, remote_ip));
                                                        do_read();
                                                    }catch (std::exception& e){
                                                        stop();
                                                    }
                                                }
                                            }
                                            else if (result == request_parser::bad)
                                            {
                                                stop();
                                            }
                                            else
                                            {
                                                do_read();
                                            }
                                        }
                                        else if (ec != boost::asio::error::operation_aborted)
                                        {
                                            connection_manager_.stop(shared_from_this());
                                        }
                                    });
        }

        void connection::do_write(boost::asio::const_buffer buf)
        {
            auto self(shared_from_this());
            boost::asio::async_write(socket_, buf,
                                     [this, self](boost::system::error_code ec, std::size_t)
                                     {
                                         if (!ec)
                                         {
                                             // Initiate graceful connection closure.
                                             boost::system::error_code ignored_ec;
                                             socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                                                              ignored_ec);
                                         }

                                         if (ec != boost::asio::error::operation_aborted)
                                         {
                                             connection_manager_.stop(shared_from_this());
                                         }
                                     });
        }

    } // namespace server
} // namespace mbus