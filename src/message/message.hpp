#include <string>
#include <vector>
#include <boost/asio.hpp>

using namespace std;
namespace mbus{
    namespace message{
        class message{
        public:
            const static REQUEST = 0;
            const static HEARTBEAT = 1;
            const static DESIP_NOT_EXIT = 2;

            int type;
            int des_ip;
            int des_index;

            int src_ip;
            int src_index;

            int request_id;

            int length;
            string data;

        public:
            message(const message&) = delete;
            message& operator=(const message&) = delete;
            message();
            message(message &&msg);
            vector<boost::asio::const_buffer> encode();
            std::string encode_string();
        };
    }
}