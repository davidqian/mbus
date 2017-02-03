//
// Created by playcrab on 1/20/17.
//

#ifndef MBUS_MSG_QUEUE_HPP
#define MBUS_MSG_QUEUE_HPP
#include <boost/interprocess/ipc/message_queue.hpp>
#include "msg_queue_manager.hpp"
using namespace boost::interprocess::ipc;
namespace mbus {
    namespace share {
        class msg_queue
                : public std::enable_shared_from_this<msg_queue>
        {
        public:
            msg_queue(const msg_queue&) = delete;
            msg_queue& operator=(const msg_queue&) = delete;

            explicit msg_queue(message_queue mq);
        public:
            message_queue mq_;

        };
        typedef std::shared_ptr<msg_queue> msg_queue_ptr;
    }
}
#endif //MBUS_MSG_QUEUE_HPP