#include <boost/lockfree/queue.hpp>
#include "message.hpp"
#include "io_message.hpp"

namespace mbus{
        class message_parser{
        public:
            message_parser();

            static message parse_string(std::string& str);

            template <typename InputIterator>
            void parse_some(boost::lockfree::queue<std::string&, boost::lockfree::fixed_sized<false>> &receiveQueue, InputIterator begin ,std::size_t bufSize){
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
                            receiveQueue.push(msg_.combination());
                            state_ = length;
                            parsed_chars = 0;
                        }
                    }else{
                        switch (state_){
                            case length:
                                tmp_parse_str[parsed_chars] = *begin++;
                                if(parsed_chars == 24){
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