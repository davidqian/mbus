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
                : state_(header)
        {
        }

        void message_parser::reset()
        {
            state_ = header;
        }

        message_parser::result_type message_parser::consume(message& msg, char input)
        {
            switch (state_)
            {
                case header:
                    if (!is_char(input) || !is_header(input))
                    {
                        return bad;
                    }
                    else
                    {
                        msg.header = (int)input;
                        msg.data = "";
                        state_ = length_start;
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
                            msg.data_length = std::stoi(msg.data_length_str);
                            msg.data_length_chars == 0;
                            msg.data_length_str = "";
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
                            msg.ip = std::stoi(msg.ip_str);
                            msg.ip_chars == 0;
                            msg.ip_str = "";
                            state_ = ip_end;
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