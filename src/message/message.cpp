#include "message.hpp"

namespace mbus{
    namespace message{
        message::message()
            :type(0),
             des_ip(0),
             des_index(0),
             src_ip(0),
             src_index(0),
             request_id(0),
             length(0),
             data("") {

        }

        message::message(message &&msg) {
            type = msg.type;
            des_ip = msg.des_ip;
            des_index = msg.des_index;
            src_ip = msg.src_ip;
            src_index = msg.src_index;
            request_id = msg.request_id;

            length = msg.length;
            data = std::move(msg.data);
        }

        vector<boost::asio::const_buffer> message::encode() {
            vector<boost::asio::const_buffer> buffers;
            buffers.push_back(boost::asio::buffer(encode_string()));
            return buffers;
        }

        std::string encode_string() {
            std::string str;
            str.reserve(25 + length);
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

            str.push_back(1,length >> 24);
            str.push_back(1,length >> 16);
            str.push_back(1,length >> 8);
            str.push_back(1,length);

            str.push_back(data.c_str());

            return str;
        }
    }
}