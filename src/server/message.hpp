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
        struct message
        {
        	int id;
        	int id_chars;
        	std::string id_str;

			int ip;
            std::string ip_chars;
            std::string ip_str;

            int data_length;
            int data_length_chars;
            std::string data_length_str;

            std::string data;
        };

    } // namespace server
} // namespace mbus

#endif // MBUS_REQUEST_HPP