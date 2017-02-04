#ifndef MBUS_IO_MESSAGE_HPP
#define MBUS_IO_MESSAGE_HPP
namespace mbus{
    class io_message{
    public:
        const static int REQUEST = 0;
        const static int HEARTBEAT = 1;
        const static int DESIP_NOT_EXIT = 2;

    public:
        int length;
        std::string body;

    public:
        std::string combination();
        std::string get_body();

        static int get_type_from_raw(std::string &msg);
        static int get_des_ip_from_raw(std::string &msg);
        static int get_des_index_from_raw(std::string &msg);

        static int get_src_ip_from_raw(std::string &msg);
        static int get_src_index_from_raw(std::string &msg);
    };
}
#endif