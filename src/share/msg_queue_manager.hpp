//
// Created by playcrab on 1/20/17.
//

#ifndef MBUS_MSG_QUEUE_MANAGER_HPP
#define MBUS_MSG_QUEUE_MANAGER_HPP
#include <boost/interprocess/ipc/message_queue.hpp>
#include "msg_queue.hpp"
using namespace boost::interprocess::ipc;
namespace mbus {
        class msg_queue_manager
        {
        public:
            void remove(std::string key);
            bool find_or_add(std::string key, message_queue &mq);
        public:
            std::map<std::string, msg_queue_ptr> mqs_;
        };
}
#endif //MBUS_MSG_QUEUE_MANAGER_HPP
