//
// Created by playcrab on 1/20/17.
//

#ifndef MBUS_MSG_QUEUE_MANAGER_HPP
#define MBUS_MSG_QUEUE_MANAGER_HPP
#include <boost/interprocess/ipc/message_queue.hpp>
#include "share/msg_queue.hpp"
#include "message/io_message.hpp"

using namespace boost::interprocess;
namespace mbus {
        class msg_queue_manager
        {
        public:
	    explicit msg_queue_manager(); 
	    ~msg_queue_manager();

            void remove(std::string &key);
            bool find(std::string &key, msg_queue_ptr &mq);
            void add(std::string &key);
        public:
            std::map<std::string, msg_queue_ptr> mqs_;
        };
}
#endif //MBUS_MSG_QUEUE_MANAGER_HPP
