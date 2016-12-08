//
// request_parser.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MBUS_MESSAGE_ENCODE_HPP
#define MBUS_MESSAGE_ENCODE_HPP

#include <vector>
#include <boost/asio.hpp>
#include "message.hpp"

namespace mbus {
    namespace server {
	namespace message_encode
        {
		std::vector<boost::asio::const_buffer> to_buffers(message& msg, int remote_ip);
	}
    }
}

#endif //MBUS_MESSAGE_ENCODE_HPP
