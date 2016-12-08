//
// request_parser.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "message_encode.hpp"

namespace mbus {
    namespace server {
	namespace message_encode
        {
		std::vector<boost::asio::const_buffer> to_buffers(message& msg, int remote_ip)
		{
			std::vector<boost::asio::const_buffer> buffers;
            		buffers.push_back(boost::asio::buffer(std::to_string(msg.header)));
            		buffers.push_back(boost::asio::buffer(std::to_string(msg.data_length)));
            		buffers.push_back(boost::asio::buffer(std::to_string(remote_ip)));
            		buffers.push_back(boost::asio::buffer(msg.data));
            		return buffers;
		}
	}
    }
}
