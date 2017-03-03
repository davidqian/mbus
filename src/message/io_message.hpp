#ifndef MBUS_IO_MESSAGE_HPP
#define MBUS_IO_MESSAGE_HPP
#include <string>
namespace mbus{
    class io_message{
    public:
        const static int REQUEST = 0;
        const static int HEARTBEAT = 1;
        const static int DESIP_NOT_EXIT = 2;
		const static int CLIENT_NOT_CONNECT = 3;
		const static int UNKONW_ERROR = 4;

    public:
        int length;
        std::string body;

    public:
	io_message(const io_message&) = delete;
        io_message& operator=(const io_message&) = delete;
        io_message();
	io_message(io_message && iomsg);	

        std::string combination();
        std::string get_body();

        static int get_type_from_raw(std::string &msg);
        static int get_des_ip_from_raw(std::string &msg);
        static int get_des_index_from_raw(std::string &msg);

        static int get_src_ip_from_raw(std::string &msg);
        static int get_src_index_from_raw(std::string &msg);

        static int get_request_id_from_raw(std::string &msg);

	static void get_data_from_raw(std::string &msg, std::string &data);
    };
}
#endif
