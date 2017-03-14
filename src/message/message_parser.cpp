#include "message_parser.hpp"
#include "message.hpp"
#include "util/util.hpp"
namespace mbus{
        message_parser::message_parser():state_(length),msg_(),parsed_chars(0)
        {

        }

        void message_parser::parse_string(std::string& str, message& msg) {
            int len = str.length() - 21;
            msg.type = (int)str[0];
            msg.des_ip = chars2int(str, 1, 4);
            msg.des_index = chars2int(str, 5, 4);
            msg.src_ip = chars2int(str, 9, 4);
            msg.src_index = chars2int(str, 13, 4);
            msg.request_id = chars2int(str, 17, 4);
            msg.data.reserve(len);
            msg.data = str.substr(21, len);
        }
}
