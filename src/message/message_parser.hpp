#ifndef MBUS_MESSAGEPARSER_HPP
#define MBUS_MESSAGEPARSER_HPP
#include <iostream>
#include <boost/lockfree/queue.hpp>
#include "message.hpp"
#include "io_message.hpp"
#include "util/util.hpp"
#include "share/my_queue.hpp"

namespace mbus{
        class message_parser{
        public:
            message_parser();

            static void parse_string(std::string& str, message& msg);

            template <typename InputIterator>
            void parse_some(my_queue &receiveQueue, InputIterator begin ,std::size_t bufSize){
                std::size_t i = 0;
                while(i < bufSize){
                    if(state_ == lengthEnd){
                        if(bufSize >= i + msg_.length){
                            msg_.body = std::string(begin, msg_.length);
                            *begin += msg_.length;
                            i += msg_.length;
                        }else{
                            msg_.body += std::string(begin, bufSize - i);
                            *begin += bufSize - i;
                            i = bufSize;
                        }

                        if(msg_.body.length() == msg_.length){
			    std::string str = std::move(msg_.body);
			    std::cout << "receive str = " << str << std::endl;
                            receiveQueue.push(str);
                            state_ = length;
                            parsed_chars = 0;
                        }
                    }else{
                        switch (state_){
                            case length:
                                tmp_parse_str[parsed_chars] = *begin++;
                                if(parsed_chars == 3){
                                    msg_.length = bigBys2Uint32(tmp_parse_str);
                                    state_ = lengthEnd;
                                }
                                break;
                        }
                        parsed_chars ++;
                        i++;
                    }
                }
            }

        public:
            io_message msg_;

            enum state
            {
                length,
                lengthEnd
            } state_;

        private:
            char tmp_parse_str[4];
            int parsed_chars;
        };
}
#endif
