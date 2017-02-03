//
// Created by playcrab on 1/17/17.
//
#include "client.hpp"


namespace mbus{
    namespace client{
        client::client(const std::string &address, const std::string &port)
            :io_service_(),
             stopped_(false),
             socket_(io_service_),
             deadline_(io_service_),
             heartbeat_timer_(io_service_)
        {
            boost::asio::tcp::resolver resolver(io_service_);
            boost::asio::tcp::resolver::query query(address, port);
            start_connect(resolver.resolve(query));
        }

        void client::start_connect(boost::asio::tcp::resolver::iterator endpoint_iter)
        {
            if (endpoint_iter != boost::asio::tcp::resolver::iterator())
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
                                    boost::asio::tcp::resolver::iterator endpoint_iter)
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

                auto receiveThread = new std::thread(client::start_receive_message_queue, this);
                receiveThread->detach();

                auto consumeThread = new std::thread(client::consume_read_queue_thread, this);
                consumeThread->detach();
            }
        }

        void client::start_receive_message_queue(client *clientPtr)
        {
            message_queue mq(open_or_create_t, "mbus_receive_message_queue", 10000, 65536);
            while(true){
                std::string msg_str;
                unsigned int priority;
                message_queue::size_type recvd_size;
                mq.receive(&msg_str, 65536, recvd_size, priority);

                message msg = clientPtr->msg_parser(msg_str);

                clientPtr->add_wirte_queue(msg);
            }
        }

        void client::start_read(const boost::system::error_code& error)
        {

            deadline_.expires_from_now(boost::posix_time::seconds(30));
            socket_.async_read_some(boost::asio::buffer(buffer_),boost::bind(&client::handle_read, this, _1, _2));
        }

        void client::handle_read(const boost::system::error_code &error,std::size_t bytes_transferred)
        {
            if (stopped_)
                return;

            if (!ec)
            {
                msg_parser.parse_some(read_que_, buffer_.data(), bytes_transferred);
                start_read();
            }
            else
            {
                stop();
            }
        }

        void client::add_read_queue(mssage msg)
        {
            read_que_.push(msg);
        }

        void client::consume_read_queue_thread(client *clientPtr)
        {
            while(true)
            {
                if(clentPtr->get_status()){
                    break;
                }
                clentPtr->consume_read_queue();
            }
        }

        void client::consume_read_queue()
        {
            message msg;
            if(read_que_.pop(msg)) {
                string processMsgQueueName = "process_message_queue_" + msg.des_index;
                msg_queue_ptr mq_prt;
                bool ret =msg_queue_manager_.find_or_add(processMsgQueueName, mq_prt);
                if(ret){
                    std::string msg_str = msg.encode_string();
                    mq_prt->mq.send(msg_str, msg_str.length, 0);
                }
                //todo err
            } else {
                std::unique_lock<std::mutex> lk(read_m_);
                read_cv_.wait(lk, [] {return !read_que_.empty();});
                read_cv_.notify_one();
            }
        }

        void client::add_wirte_queue(message msg)
        {
            if(msg.src_ip == msg.des_ip){
                read_que_.push(msg);
            }else {
                wirte_que_.push(msg.encode());
            }
        }

        void client::consume_write_queue()
        {
            boost::asio::buffer buf;
            if(wirte_que_.pop(buf)) {
                start_write(buf);
            } else {
                std::unique_lock<std::mutex> lk(write_m_);
                write_cv_.wait(lk, [] {return !wirte_que_.empty();});
                write_cv_.notify_one();
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

        void client::start_write(boost::asio::buffer buf)
        {
            if (stopped_)
                return;


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
            hb_msg_.type = util::HEART_BEAT;
            hb_msg_.src_ip = ip_;
            hb_msg_.des_index = 0;
            hb_msg_.src_index = 0;
            hb_msg_.data = "hb";
            add_wirte_queue(hb_msg_);
        }

        void client::check_deadline()
        {
            if (stopped_)
                return;

            if (deadline_.expires_at() <= deadline_timer::traits_type::now())
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

        void client::get_status()
        {
            return stopped_;
        }
    }
}
