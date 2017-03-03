//
// Created by playcrab on 1/20/17.
//
#include "msg_queue_manager.hpp"
#include "message/message.hpp";

namespace mbus {
	msg_queue_manager::msg_queue_manager(){}

	msg_queue_manager::~msg_queue_manager(){
            mqs_.clear();
	}

        bool msg_queue_manager::find(std::string &key, msg_queue_ptr &mq_ptr)
        {
            mq_ptr = nullptr;
            std::map<std::string, msg_queue_ptr>::iterator iter;
            iter = mqs_.find(key);
            if(iter != mqs_.end()){
                mq_ptr = iter->second;
                return true;
            }else{
		return false;
            }
        }

	bool msg_queue_manager::find_or_add(std::string &key, msg_queue_ptr &mq_ptr)
	{
	    mq_ptr = nullptr;
            std::map<std::string, msg_queue_ptr>::iterator iter;
            iter = mqs_.find(key);
            if(iter != mqs_.end()){
                mq_ptr = iter->second;
                return true;
            }else{
		try{
		   std::cout << "key = " << key << std::endl;
                   mq_ptr = std::make_shared<msg_queue>(key);
                   mq_ptr->open_queue();
                   mqs_[key] = mq_ptr;	
                   return true;
		}catch(interprocess_exception &e){
	   	   std::cout << "exception = " << e.what() << std::endl;
		   return false;
		}
            }
	}

        void msg_queue_manager::add(std::string &key)
        {
		msg_queue_ptr mq_ptr;
		mq_ptr = std::make_shared<msg_queue>(key);
		mq_ptr->open_queue();
		mqs_[key] = mq_ptr;
	}

        void msg_queue_manager::remove(std::string &key)
        {
	    std::map<std::string, msg_queue_ptr>::iterator iter;
            iter = mqs_.find(key);
	    if(iter != mqs_.end()){
               mqs_.erase(key);
	    }
        }
}
