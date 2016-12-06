//
// request_parser.cpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "message_parser.hpp"
#include "message.hpp"

namespace mbus {
    namespace server {

        message_parser::message_parser()
                : state_(header_start)
        {
        }

        void message_parser::reset()
        {
            state_ = header_start;
        }

        message_parser::result_type message_parser::consume(message& msg, char input)
        {
            switch (state_)
            {
                case header_start:
                    if (!is_char(input) || !is_header(input))
                    {
                        return bad;
                    }
                    else
                    {
                        state_ = id_start;
                        return indeterminate;
                    }
                case id_start:
                    if (!is_char(input))
                    {
                        return bad;
                    }
                    else
                    {
                        msg.id_str.push_back(input);
                        msg.id_chars += 1;
                        if(msg.id_chars == 4){
                            state_ = ip_start;
                        }
                        return indeterminate;
                    }
                case ip_start:
                    if (!is_char(input))
                    {
                        return bad;
                    }
                    else
                    {
                        msg.ip_str.push_back(input);
                        msg.ip_chars += 1;
                        if(msg.ip_chars == 4){
                            state_ = length_start;
                        }
                        return indeterminate;
                    }
                case length_start:
                    if (!is_char(input))
                    {
                        return bad;
                    }
                    else
                    {
                        msg.data_length_str.push_back(input);
                        msg.data_length_chars += 1;
                        if(msg.data_length_chars == 4){
                            state_ = length_end;
                        }
                        return indeterminate;
                    }
                default:
                    return bad;
            }
        }

        bool message_parser::is_char(int c){
            return c >= 0 && c <= 127;
        }

        bool message_parser::is_header(char c)
        {
            int header = (int)c;
            if(header == 1 || header == 2){
                return true;
            }
            return false;
        }

    } // namespace server
} // namespace mbus