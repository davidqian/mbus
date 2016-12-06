//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MBUS_MESSAGE_HANDLER_HPP
#define MBUS_MESSAGE_HANDLER_HPP

#include <string>

namespace mbus {
    namespace server {

        struct message;

/// The common handler for all incoming requests.
        class message_handler
        {
        public:
            message_handler(const message_handler&) = delete;
            message_handler& operator=(const message_handler&) = delete;

            explicit message_handler(const std::string& message_buf);

            /// Handle a message and produce a reply.
            void handle_message(const message& msg);

        private:
            std::string message_buf_;

        };

    } // namespace server
} // namespace mbus

#endif // MBUS_MESSAGE_HANDLER_HPP