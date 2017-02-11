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

        void message::encode_string(std::string& str) {
            str.reserve(21 + data.length());
            str.append(1, type);

            str.append(1,des_ip >> 24);
            str.append(1,des_ip >> 16);
            str.append(1,des_ip >> 8);
            str.append(1,des_ip);

            str.append(1,des_index >> 24);
            str.append(1,des_index >> 16);
            str.append(1,des_index >> 8);
            str.append(1,des_index);

            str.append(1,src_ip >> 24);
            str.append(1,src_ip >> 16);
            str.append(1,src_ip >> 8);
            str.append(1,src_ip);

            str.append(1,src_index >> 24);
            str.append(1,src_index >> 16);
            str.append(1,src_index >> 8);
            str.append(1,src_index);

            str.append(1,request_id >> 24);
            str.append(1,request_id >> 16);
            str.append(1,request_id >> 8);
            str.append(1,request_id);

            str.append(data.c_str());
        }

}
