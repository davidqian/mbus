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
#include "message/message.hpp"
#include "message/message_parser.hpp"
#include "share/msg_queue.hpp"
#include "share/msg_queue_manager.hpp"
namespace mbus {
    namespace client {
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

            void add_wirte_queue(message msg);

            void consume_write_queue_thread(client *clentPtr);

            void consume_write_queue();

            void add_read_queue(message msg);

            void consume_read_queue_thread(client *clientPtr);

            void consume_read_queue();

            void get_status();


        private:
            void start_connect(boost::asio::tcp::resolver::iterator endpoint_iter);
            void handle_connect(const boost::system::error_code& ec,
                                boost::asio::tcp::resolver::iterator endpoint_iter);
            void start_write(boost::asio::buffer buf);
            void handle_write(const boost::system::error_code& error);
            void handle_read(const boost::system::error_code& error);
            void handle_read(const boost::system::error_code& ec);
            void start_read();
            void check_deadline();
            void heart_beat();
            void stop();


        private:
            bool stopped_;
            boost::asio::io_service io_service_;
            boost::asio::tcp::socket socket_;
            std::array<char, 8192> buffer_;
            boost::asio::deadline_timer deadline_;
            boost::asio::deadline_timer heartbeat_timer_;
            message hb_msg_;
            message msg_;
            boost::lockfree::queue<message, fixed_sized<false> > wirte_que_;
            boost::lockfree::queue<message, fixed_sized<false> > read_que_;
            std::mutex read_m_;
            std::condition_variable read_cv_;

            std::mutex write_m_;
            std::condition_variable write_cv_;

            msg_queue_manager msg_queue_manager_;

            message_parser msg_parser;

            long ip_;
        };
    }
}
#endif //MBUS_CLIENT_HPP
