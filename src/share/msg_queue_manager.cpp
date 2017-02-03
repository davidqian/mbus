//
// Created by playcrab on 1/20/17.
//
#include "msg_queue_manager.hpp"

namespace mbus {
    namespace share {
        bool msg_queue_manager::find_or_add(std::string key, msg_queue_ptr &mq_ptr)
        {
            mq_ptr = nullptr;
            std::map<std::string, msg_queue_ptr>::iterator iter;
            iter = mqs_.find(key);
            if(iter != mqs_.end()){
                mq_ptr = iter->second;
                return true;
            }else{
                try{
                    message_queue mq
                            (open_only,
                             key
                            );
                    mq_ptr = std::make_shared<msg_queue>(std::move(mq));
                    mqs_[key] = mq_ptr;
                    return true;
                }catch (interprocess_exception &ex){
                    return false;
                }
            }
        }

        void msg_queue_manager::remove(std::string key)
        {
            mqs_.erase(key);
        }
    }
}
