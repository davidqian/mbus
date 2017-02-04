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

namespace mbus {

        connection::connection(boost::asio::ip::tcp::socket socket,
                               connection_manager& manager)
                : socket_(std::move(socket)),
                  connection_manager_(manager),
                  long_ip_(0),
                  heartbeat_time_(time(nullptr))
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
                                            message_parser_.parse_some(connection_manager_.msg_que_, buffer_.data(), bytes_transferred);
                                            do_read();
                                        }
                                        else if (ec != boost::asio::error::operation_aborted)
                                        {
                                            connection_manager_.stop(shared_from_this());
                                        }
                                    });
        }

        void connection::do_write(std::string &str)
        {
            int str_len = str.length();
            std::string len;
            len.reserve(4);
            len.push_back(1, str_len >> 24);
            len.push_back(1, str_len >> 16);
            len.push_back(1, str_len >> 8);
            len.push_back(1, str_len);

            std::vector<boost::asio::const_buffer> buf;
            buf.push_back(boost::asio::buffer(len));
            buf.push_back(boost::asio::buffer(str));

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
                                             stop();
                                         }
                                     });
        }
	
      	int connection::get_remote_ip()
      	{
            if(long_ip_ == 0){
                long_ip_ = util::ip2long(socket_.remote_endpoint().address().to_string());
            }
            return long_ip_;
      	}

} // namespace mbus
