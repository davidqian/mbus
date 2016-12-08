//
// request_parser.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MBUS_REQUEST_PARSER_HPP
#define MBUS_REQUEST_PARSER_HPP

#include <tuple>
#include "message.hpp"

namespace mbus {
    namespace server {

        struct message;

/// Parser for incoming requests.
        class message_parser
        {
        public:
            /// Construct ready to parse the request method.
            message_parser();

            /// Reset to initial parser state.
            void reset();

            /// Result of parse.
            enum result_type { good, bad, indeterminate };

            /// Parse some data. The enum return value is good when a complete request has
            /// been parsed, bad if the data is invalid, indeterminate when more data is
            /// required. The InputIterator return value indicates how much of the input
            /// has been consumed.
            template <typename InputIterator>
            std::tuple<result_type, InputIterator> parse(message& msg,
                                                         InputIterator begin, InputIterator end)
            {
                while (begin != end)
                {
                    if (state_ == ip_end) {
                        msg.data = std::string(begin, end);
                        if (msg.data.length() < msg.data_length) {
                            return std::make_tuple(indeterminate, begin);
                        } else if (msg.data.length() > msg.data_length) {
                            return std::make_tuple(bad, begin);
                        } else {
                            return std::make_tuple(good, begin);
                        }
                    }

                    result_type result = consume(msg, *begin++);
                    if (result == bad) {
                        return std::make_tuple(bad, begin);
                    }
                }
                return std::make_tuple(indeterminate, begin);
            };

            /// The current state of the parser.
            enum state
            {
                header,
                length_start,
                ip_start,
                ip_end
            } state_;

        private:
            /// Handle the next character of input.
            result_type consume(message& msg, char input);

            static bool is_header(char c);

            static bool is_char(int c);

        };

    } // namespace server
} // namespace mbus

#endif // MBUS_REQUEST_PARSER_HPP
