//
// Created by playcrab on 1/17/17.
//

#ifndef MBUS_TEST_HPP
#define MBUS_TEST_HPP

#include <boost/asio/deadline_timer.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "message/message.hpp"
#include "message/message_parser.hpp"
using namespace boost::interprocess;
namespace mbus {
        class test
        {
        public:
            test(const test&) = delete;
            test& operator=(const test&) = delete;

            /// Construct the server to listen on the specified TCP address and port, and
            /// serve up files from the given directory.
            test(int index);
	    ~test();

	    void run();
	    
	    void consume_read_queue();

	    void write(std::string &msg);

        private:
            static void consume_read_queue_thread(test *testPtr);

        private:
	    message_queue* write_mq_;
	    message_queue* read_mq_;

            std::mutex read_m_;
            std::condition_variable read_cv_;

            std::mutex write_m_;
            std::condition_variable write_cv_;
	    
	    int index_;
            int ip_;
        };
}
#endif //MBUS_CLIENT_HPP
