#include "io_message.hpp"
#include "util/util.hpp"
namespace mbus{
    io_message::combination() {
            std::string str;
            str.reserve(4 + length);
            str.push_back(1,length >> 24);
            str.push_back(1,length >> 16);
            str.push_back(1,length >> 8);
            str.push_back(1,length);
            str.push_back(body.c_str());
            return str;
    }

    std::string io_message::get_body() {
        return std::move(body);
    }

    static int io_message::get_type_from_raw(std::string &msg) {
        return chars2int(msg, 0,1);
    }

    static int io_message::get_des_ip_from_raw(std::string &msg) {
        return chars2int(msg, 1, 4);
    }

    static int io_message::get_des_index_from_raw(std::string &msg) {
        return chars2int(msg, 5, 4);
    }

    static int io_message::get_src_ip_from_raw(std::string &msg) {
        return chars2int(msg, 9, 4);
    }

    static int io_message::get_src_index_from_raw(std::string &msg) {
        return chars2int(msg, 13, 4);
    }
}