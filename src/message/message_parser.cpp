#include "message_parser.hpp"

namespace mbus{
    namespace message{
        message_parser::message_parser()
            :state_(type),
             msg_(),
             parsed_chars(0)
        {

        }

        message message_parser::parse_string(std::string str) {
            message msg;
            msg.type = (int)str[0];
            msg.des_ip = chars2int(str, 1, 4);
            msg.des_index = chars2int(str, 5, 4);
            msg.src_ip = chars2int(str, 9, 4);
            msg.src_index = chars2int(str, 13, 4);
            msg.request_id = chars2int(str, 17, 4);
            msg.length = chars2int(str, 21, 4);
            msg.data.reserve(msg.length);
            msg.data = str.substr(25,msg.length);

            return msg;
        }

        int message_parser::chars2int(std::string str, int begin, int length) {
            char chars[length];
            int i = 0;
            for(i = 0; i < length; i++){
                chars[i] = str[begin+i];
            }
            return bigBys2Uint32(chars);
        }

        int message_parser::bigBys2Uint32(const char bys[4]) {
            int uint = 0;
            for(int i = 0; i <= 3; i++){
                uint |= (0xff & bys[i]) << (8*(3-i));
            }
            return uint;
        }
    }
}