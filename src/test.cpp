//
// Created by playcrab on 1/13/17.
//
#include <boost/interprocess/ipc/message_queue.hpp>
#include <string>
#include <unistd.h>
#include "test/test.hpp"
using namespace boost::interprocess;
int main(int argc, const char * argv[]){
        message_queue::remove("process_message_queue_1");
        message_queue *mq = new message_queue(open_only, "process_message_queue_1"); 
	std::string str("abc");
	mq->send(str.data(),str.size(),0);
        /**
        mbus::test t(2);
	t.run();
	while(true){
		std::string str("abc");
		t.write(str);
		sleep(10);
	}
        **/
}

