//
// Created by playcrab on 1/17/17.
//
#include "client.hpp"
#include "message/io_message.hpp"
using namespace boost::interprocess;
namespace mbus{
        client::client(const std::string &address, const std::string &port)
            :io_service_(),
             stopped_(false),
             socket_(io_service_),
             deadline_(io_service_),
             heartbeat_timer_(io_service_),
	     ip_(2130706433)
        {
            boost::asio::ip::tcp::resolver resolver(io_service_);
            boost::asio::ip::tcp::resolver::query query(address, port);
            start_connect(resolver.resolve(query));
        }

        void client::start_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iter)
        {
            if (endpoint_iter != boost::asio::ip::tcp::resolver::iterator())
            {
                deadline_.expires_from_now(boost::posix_time::seconds(60));

                socket_.async_connect(endpoint_iter->endpoint(),
                                      boost::bind(&client::handle_connect,
                                                  this, _1, endpoint_iter));
            }
            else
            {
                stop();
            }
        }

        void client::run()
        {
            io_service_.run();
        }

        void client::handle_connect(const boost::system::error_code& ec,
                                    boost::asio::ip::tcp::resolver::iterator endpoint_iter)
        {
            if (stopped_)
                return;

            if (!socket_.is_open())
            {
                start_connect(++endpoint_iter);
            }
            else if (ec)
            {
                socket_.close();
                start_connect(++endpoint_iter);
            }
            else
            {
                start_read();

                heartbeat_timer_.expires_from_now(boost::posix_time::seconds(10));
                heartbeat_timer_.async_wait(boost::bind(&client::heart_beat, this));

		auto messageQueueThread = new std::thread(client::consume_message_queue_thread, this);
                messageQueueThread->detach();

                auto writeThread = new std::thread(client::consume_write_queue_thread, this);
                writeThread->detach();

                auto readThread = new std::thread(client::consume_read_queue_thread, this);
                readThread->detach();
            }
        }

        void client::consume_message_queue_thread(client *clientPtr)
        {
	    open_or_create_t openOrCreate;
            message_queue mq(openOrCreate,"mbus_receive_message_queue",10000,65536);
            while(true){
                std::string msg_str;
                unsigned int priority;
                message_queue::size_type recvd_size;
                mq.receive(&msg_str, 65536, recvd_size, priority);
                clientPtr->add_wirte_queue(msg_str);
            }
        }

        void client::start_read()
        {
	    buffer_.data()[0] = '\0'; 
            socket_.async_read_some(boost::asio::buffer(buffer_), boost::bind(&client::handle_read, this, boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
        }

        void client::handle_read(const boost::system::error_code& error,std::size_t bytes_transferred)
        {
            if (stopped_)
                return;

            if (!error)
            {
                msg_parser.parse_some(read_que_, buffer_.data(), bytes_transferred);
                start_read();
            }
            else if(error != boost::asio::error::operation_aborted)
            {
		std::cout << "error code " << error << error.message() << std::endl;
                stop();
            }
        }

        void client::consume_read_queue_thread(client *clientPtr)
        {
            while(true)
            {
                if(clientPtr->get_status()){
                    break;
                }
                clientPtr->consume_read_queue();
            }
        }

        void client::consume_read_queue()
        {
            std::string msg;
            if(read_que_.pop(msg)) {
                std::cout << "msg " << msg << " msg len " << msg.length() << std::endl;
		int ty = io_message::get_type_from_raw(msg);
                std::cout << "type = " << ty << std::endl;
		if(ty != io_message::HEARTBEAT){
                    string processMsgQueueName = "process_message_queue_" + io_message::get_des_index_from_raw(msg);
                    msg_queue_ptr mq_ptr;
                    bool ret = msg_queue_manager_.find_or_add(processMsgQueueName, mq_ptr);
                    if(ret){
                        mq_ptr->mq_.send(msg.c_str(), msg.size(), 0);
                    }
                }
            }
        }

        void client::add_wirte_queue(std::string& msg)
        {
            if(ip_ == io_message::get_des_ip_from_raw(msg)){
                read_que_.push(msg);
            }else {
                wirte_que_.push(msg);
            }
        }

        void client::consume_write_queue()
        {
            std::string str;
            if(wirte_que_.pop(str)) {
                start_write(str);
            }
        }

        void client::consume_write_queue_thread(client *clentPtr)
        {
            while(true)
            {
                if(clentPtr->get_status()){
                    break;
                }
                clentPtr->consume_write_queue();
            }
        }

        void client::start_write(std::string &str)
        {
            if (stopped_)
                return;
            int str_len = str.length();
	    std::cout << "write str " << str << "str len " << str_len << std::endl;
            std::string len;
            len.reserve(4);
            len.append(1, str_len >> 24);
            len.append(1, str_len >> 16);
            len.append(1, str_len >> 8);
            len.append(1, str_len);
	    
            std::vector<boost::asio::const_buffer> buf;
            buf.push_back(boost::asio::buffer(len));
            buf.push_back(boost::asio::buffer(str));

            boost::system::error_code ec;
            socket_.write_some(buf, ec);

            if(ec){
                stop();
                return;
            }

            heartbeat_timer_.expires_from_now(boost::posix_time::seconds(10));
            heartbeat_timer_.async_wait(boost::bind(&client::heart_beat, this));
        }

        void client::handle_write(const boost::system::error_code& ec)
        {
            if (stopped_)
                return;

            if (ec)
            {
                stop();
            }
        }

        void client::heart_beat()
        {
            hb_msg_.type = io_message::HEARTBEAT;
            hb_msg_.des_index = 0;
            hb_msg_.src_ip = ip_;
            hb_msg_.src_index = 0;
            hb_msg_.data = "hb";
	    std::string hbmsg;
	    hb_msg_.encode_string(hbmsg);
            add_wirte_queue(hbmsg);
        }

        void client::check_deadline()
        {
            if (stopped_)
                return;

            if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
            {
                socket_.close();
                deadline_.expires_at(boost::posix_time::pos_infin);
            }
            deadline_.async_wait(boost::bind(&client::check_deadline, this));
        }

        void client::stop()
        {
            stopped_ = true;
            socket_.close();
            deadline_.cancel();
            heartbeat_timer_.cancel();
        }

        bool client::get_status()
        {
            return stopped_;
        }
}
