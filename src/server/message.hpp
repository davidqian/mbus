//
// message.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MBUS_REQUEST_HPP
#define MBUS_REQUEST_HPP

#include <string>

namespace mbus {
    namespace server {

        /// A message received from a client.
        /// message format length|data
        /// data format header|length|ip|index|id|message_data

        struct message
        {
            int header;
            /// message length
            int data_length;
            int data_length_chars;
            std::string data_length_str;

            int ip;
            int ip_chars;
            std::string ip_str;

            /// message data
            std::string data;
        };

        std::vector<boost::asio::const_buffer> to_buffers(message& msg, int remote_ip)
        {
            std::vector<boost::asio::const_buffer> buffers;
            buffers.push_back(boost::asio::buffer(std::to_string(msg.header)));
            buffers.push_back(boost::asio::buffer(std::to_string(msg.data_length)));
            buffers.push_back(boost::asio::buffer(std::to_string(remote_ip)));
            buffers.push_back(boost::asio::buffer(msg.data));
            return buffers;
        }

    } // namespace server
} // namespace mbus

#endif // MBUS_REQUEST_HPP