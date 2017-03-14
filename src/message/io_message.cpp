#include "io_message.hpp"
#include "util/util.hpp"
namespace mbus{
    io_message::io_message():length(0),body(""){
	   }

    io_message::io_message(io_message&& iomsg):length(iomsg.length),body(std::move(iomsg.body)){
	     iomsg.body = nullptr;
    }

    std::string io_message::combination() {
            std::string str;
            str.reserve(4 + length);
            str.append(1,length >> 24);
            str.append(1,length >> 16);
            str.append(1,length >> 8);
            str.append(1,length);
            str.append(body.c_str());
            return str;
    }

    std::string io_message::get_body() {
        return std::move(body);
    }

    int io_message::get_type_from_raw(std::string &msg) {
	     return (int)msg[0];
    }

    int io_message::get_des_ip_from_raw(std::string &msg) {
        return chars2int(msg, 1, 4);
    }

    int io_message::get_des_index_from_raw(std::string &msg) {
        return chars2int(msg, 5, 4);
    }

    int io_message::get_src_ip_from_raw(std::string &msg) {
        return chars2int(msg, 9, 4);
    }

    int io_message::get_src_index_from_raw(std::string &msg) {
        return chars2int(msg, 13, 4);
    }

    int io_message::get_request_id_from_raw(std::string &msg) {
	     return chars2int(msg, 17, 4);
    }

    void io_message::get_data_from_raw(std::string &msg, std::string &data) {
        data = msg.substr(21);
    }
}
