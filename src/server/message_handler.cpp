//
// message_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "message_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "message.hpp"

namespace mbus {
    namespace server {

        message_handler::message_handler(const std::string& message_buf)
                : message_buf_(message_buf)
        {
        }

        void message_handler::handle_message(const message& msg)
        {
            
        }

    } // namespace server
} // namespace mbus