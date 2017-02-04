#include "message.hpp"

namespace mbus{
        message::message()
            :type(0),
             des_ip(0),
             des_index(0),
             src_ip(0),
             src_index(0),
             request_id(0),
             data("") {
        }

        std::string message::encode_string() {
            std::string str;
            str.reserve(21 + data.length());
            str.push_back(1, type);
            str.push_back(1,des_ip >> 24);
            str.push_back(1,des_ip >> 16);
            str.push_back(1,des_ip >> 8);
            str.push_back(1,des_ip);

            str.push_back(1,des_index >> 24);
            str.push_back(1,des_index >> 16);
            str.push_back(1,des_index >> 8);
            str.push_back(1,des_index);

            str.push_back(1,src_ip >> 24);
            str.push_back(1,src_ip >> 16);
            str.push_back(1,src_ip >> 8);
            str.push_back(1,src_ip);

            str.push_back(1,src_index >> 24);
            str.push_back(1,src_index >> 16);
            str.push_back(1,src_index >> 8);
            str.push_back(1,src_index);

            str.push_back(1,request_id >> 24);
            str.push_back(1,request_id >> 16);
            str.push_back(1,request_id >> 8);
            str.push_back(1,request_id);

            str.push_back(data.c_str());

            return str;
        }

}