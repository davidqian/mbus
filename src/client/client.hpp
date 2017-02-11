//
// Created by playcrab on 1/17/17.
//

#ifndef MBUS_CLIENT_HPP
#define MBUS_CLIENT_HPP

#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/bind.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "message/message.hpp"
#include "message/message_parser.hpp"
#include "share/msg_queue.hpp"
#include "share/msg_queue_manager.hpp"
#include "share/my_queue.hpp"

using namespace boost::interprocess;
namespace mbus {
        class client
                : public std::enable_shared_from_this<client>
        {
        public:
            client(const client&) = delete;
            client& operator=(const client&) = delete;

            /// Construct the server to listen on the specified TCP address and port, and
            /// serve up files from the given directory.
            explicit client(const std::string& address, const std::string& port);

            void run();

            void add_wirte_queue(std::string& msg);

            void consume_write_queue();

            void consume_read_queue();

            bool get_status();

            void handle_write(const boost::system::error_code& ec);
            void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred);


        private:
            static void consume_message_queue_thread(client *clentPtr);
            static void consume_write_queue_thread(client *clentPtr);
            static void consume_read_queue_thread(client *clientPtr);

            void start_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iter);
            void handle_connect(const boost::system::error_code& ec,
                                boost::asio::ip::tcp::resolver::iterator endpoint_iter);
            void start_write(std::string& str);
            void start_read();
            void check_deadline();
            void heart_beat();
            void stop();


        private:
            bool stopped_;
            boost::asio::io_service io_service_;
            boost::asio::ip::tcp::socket socket_;
            std::array<char, 8192> buffer_;
            boost::asio::deadline_timer deadline_;
            boost::asio::deadline_timer heartbeat_timer_;
            message hb_msg_;
            message msg_;
            my_queue wirte_que_;
            my_queue read_que_;

            std::mutex read_m_;
            std::condition_variable read_cv_;

            std::mutex write_m_;
            std::condition_variable write_cv_;

            msg_queue_manager msg_queue_manager_;

            message_parser msg_parser;

            int ip_;
        };
}
#endif //MBUS_CLIENT_HPP
