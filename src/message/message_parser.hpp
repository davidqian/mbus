#include <boost/lockfree/queue.hpp>
#include "message.hpp"

namespace mbus{
    namespace message{
        class message_parser{
        public:
            message_parser();

            message parse_string(std::string str);

            template <typename InputIterator>
            void parse_some(boost::lockfree::queue<message, fixed_sized<false>> &receiveQueue, InputIterator begin ,std::size_t bufSize){
                std::size_t i = 0;
                while(i < bufSize){
                    if(state_ == lengthEnd){
                        if(bufSize >= i + msg_.length){
                            msg_.data = std::string(begin, msg_.length);
                            *begin += msg_.length;
                            i += msg_.length;
                        }else{
                            msg_.data += std::string(begin, bufSize - i);
                            *begin += bufSize - i;
                            i = bufSize;
                        }
                        if(msg_.data.length() == msg_.length){
                            receiveQueue.push(std::move(msg_));
                            state_ = desIp;
                            parsed_chars = 0;
                        }
                    }else{
                        switch (state_){
                            case type:
                                msg_.type = (int)*begin++;
                                state_ = desIp;
                                break;
                            case desIp:
                                tmp_parse_str[parsed_chars] = *begin++;
                                if(parsed_chars == 4){
                                    msg_.des_ip = bigBys2Uint32(tmp_parse_str);
                                    state_ = desIndex;
                                }
                                break;
                            case desIndex:
                                tmp_parse_str[(parsed_chars-5)] = *begin++;
                                if(parsed_chars == 8){
                                    msg_.des_index = bigBys2Uint32(tmp_parse_str);
                                    state_ = srcIp;
                                }
                                break;
                            case srcIp:
                                tmp_parse_str[(parsed_chars-9)] = *begin++;
                                if(parsed_chars == 12){
                                    msg_.src_ip = bigBys2Uint32(tmp_parse_str);
                                    state_ = srcIndex;
                                }
                                break;
                            case srcIndex:
                                tmp_parse_str[(parsed_chars-13)] = *begin++;
                                if(parsed_chars == 16){
                                    msg_.src_index = bigBys2Uint32(tmp_parse_str);
                                    state_ = requestId;
                                }
                                break;
                            case requestId:
                                tmp_parse_str[(parsed_chars-21)] = *begin++;
                                if(parsed_chars == 20){
                                    msg_.request_id = bigBys2Uint32(tmp_parse_str);
                                    state_ = length;
                                }
                                break;
                            case length:
                                tmp_parse_str[(parsed_chars-25)] = *begin++;
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
        private:
            int chars2int(std::string str, int begin, int length);
            int bigBys2Uint32(const char bys[4]);

        public:
            message msg_;

            enum state
            {
                type,
                desIp,
                desIndex,
                srcIp,
                srcIndex,
                requestId,
                length,
                lengthEnd
            } state_;

        private:
            char tmp_parsed_str[4];
            int parsed_chars;
        };
    }
}