//
// Created by playcrab on 1/20/17.
//

#ifndef MBUS_MY_QUEUE_HPP
#define MBUS_MY_QUEUE_HPP
#include <queue>
#include <string>
#include <mutex>
#include <iostream>
#include <condition_variable>
namespace mbus {
        class my_queue
        {
        public:
            my_queue(const my_queue&) = delete;
            my_queue& operator=(const my_queue&) = delete;
	    my_queue();

	    void push(std::string &str);
	    bool pop(std::string &str);

        public:
	     int max_queue_len_;
             std::queue<std::string> queue_;
	     std::mutex m_;
	     std::condition_variable cv_;
        };
}
#endif //MBUS_MY_QUEUE_HPP
