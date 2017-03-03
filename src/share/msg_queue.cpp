//
// Created by playcrab on 1/20/17.
//

#include "msg_queue.hpp"

namespace mbus {
	msg_queue::msg_queue(std::string& key)
	:key_(key)
        {
        }

	msg_queue::~msg_queue(){
		std::cout << "msg_queue des" << std::endl;
		delete mq_;
	}

	void msg_queue::open_queue() {
		std::cout << "msg queue key = " << key_ << std::endl;
		mq_ = new message_queue(open_only, key_.c_str());
	}
}

