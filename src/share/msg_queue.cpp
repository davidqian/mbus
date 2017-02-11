//
// Created by playcrab on 1/20/17.
//

#include "msg_queue.hpp"

namespace mbus {
	msg_queue::msg_queue(open_only_t open,const char* key)
	:mq_(open, key)
        {
        }
}

