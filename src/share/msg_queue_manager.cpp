//
// Created by playcrab on 1/20/17.
//
#include "msg_queue_manager.hpp"
#include "message/message.hpp";

namespace mbus {
	msg_queue_manager::msg_queue_manager(){}

	msg_queue_manager::~msg_queue_manager(){
	    message client;
	    client.type = io_message::CLIENT_CRASH;
            client.des_index = 0;
            client.src_ip = 0;
            client.src_index = 0;
            client.data = "client crash";
            std::string clientmsg;
            client.encode_string(clientmsg); 
	    std::map<std::string,msg_queue_ptr>::iterator it;
            for(it=mqs_.begin(); it!=mqs_.end(); ++it) {
                it->second->mq_->send(clientmsg.data(),clientmsg.size(),0);
            }
            mqs_.clear();
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
                    mq_ptr = std::make_shared<msg_queue>(key);
		    mq_ptr->open_queue();
                    mqs_[key] = mq_ptr;
                    return true;
                }catch (interprocess_exception &ex){
                    return false;
                }
            }
        }

        void msg_queue_manager::add(std::string &key)
        {
		msg_queue_ptr mq_ptr;
		std::make_shared<msg_queue>(key);
		mq_ptr->open_queue();
		mqs_[key] = mq_ptr;
	}

        void msg_queue_manager::remove(std::string &key)
        {
	    std::map<std::string, msg_queue_ptr>::iterator iter;
            iter = mqs_.find(key);
	    if(iter != mqs_.end()){
	       //delete iter->second->mq_;
               mqs_.erase(key);
	    }
        }
}
