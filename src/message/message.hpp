
#include <string>
#include <vector>
#include <boost/asio.hpp>

using namespace std;
namespace mbus{
        class message{
        public:
            int type;
            int des_ip;
            int des_index;

            int src_ip;
            int src_index;

            int request_id;

            string data;

        public:
            message(const message&) = delete;
            message& operator=(const message&) = delete;
            message();
            std::string encode_string();
        };
}