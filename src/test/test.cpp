//
// Created by playcrab on 1/17/17.
//
#include "test.hpp"
#include "message/io_message.hpp"
using namespace boost::interprocess;
namespace mbus{
	test::test(int index)
		:index_(index),
		ip_(2130706433)
	{
	}

	test::~test() {
		delete read_mq_;
	        delete write_mq_;
	}

	void test::run() {
		write_mq_ = new message_queue(open_only, "mbus_receive_message_queue");	
		std::string read_mq_name = "process_message_queue_"+std::to_string(index_);
		message_queue::remove(read_mq_name.c_str());
		read_mq_ = new message_queue(open_or_create, read_mq_name.c_str(), 10, 65535);
                auto readThread = new std::thread(test::consume_read_queue_thread, this);
                readThread->detach();
	}

	void test::consume_read_queue_thread(test *testPtr) {
		while(true){
			testPtr->consume_read_queue();
		}
	}

	void test::consume_read_queue() {
		std::string msg_str;
                unsigned int priority;
                message_queue::size_type recvd_size;
		try{
		    std::cout << "receive begin form read queue" << std::endl; 
                    read_mq_->receive(&msg_str[0], 65535, recvd_size, priority);
		    std::cout << "receive msg " << msg_str << " " << msg_str.size() << std::endl;
		}catch(interprocess_exception &ex){
		    std::cout << ex.what() << std::endl;
		}
	}

	void test::write(std::string &data) {
		message msg;
		msg.type = io_message::REQUEST;
		msg.des_ip = 2130706433;
		msg.des_index = 1;
		msg.src_ip = 2130706433;
		msg.src_index = 2;
		msg.request_id = 0;
		msg.data = data;
		std::string m;
		msg.encode_string(m);
		write_mq_->send(m.data(), m.size(), 0);
	}	
}
