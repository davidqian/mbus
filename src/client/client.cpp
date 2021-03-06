//
// Created by playcrab on 1/17/17.
//
#include <signal.h>
#include <time.h>
#include <thread>
#include "client.hpp"
#include "message/io_message.hpp"
#include "util/util.hpp"

using namespace boost::interprocess;
namespace mbus{
  client::client(const std::string &address, const std::string &port, const int ip, const std::string &shm_key)
      :io_service_(),signal_io_service_(),address_(address), port_(port),socketOpend_(false),reconnect_(true),deadline_(io_service_), signals_(signal_io_service_), heartbeat_timer_(io_service_), ip_(ip),share_memory_(shm_key)
  {
      signals_.add(SIGINT);
      signals_.add(SIGTERM);
      #if defined(SIGQUIT)
        signals_.add(SIGQUIT);
      #endif
      do_await_stop();
  }
	client::~client() {
	}

	void client::do_await_stop() {
	    signals_.async_wait(
                  [this](boost::system::error_code /*ec*/, int /*signo*/)
                  {
                      stop(true);
                  });
	}

  void client::start_connect()
  {
    while(reconnect_){
       	socket_.reset(new boost::asio::ip::tcp::socket(io_service_));
       	boost::asio::ip::tcp::resolver resolver(io_service_);
       	boost::asio::ip::tcp::resolver::query query(address_, port_);
       	boost::asio::ip::tcp::resolver::iterator endpoint_iter = resolver.resolve(query);

       	deadline_.expires_from_now(boost::posix_time::seconds(60));
       	socket_->async_connect(endpoint_iter->endpoint(),boost::bind(&client::handle_connect,this,_1));

       io_service_.run();
       socket_.reset();
       io_service_.reset();
    }
  }

  void client::handle_connect(const boost::system::error_code& ec)
  {
      if (!socket_->is_open()){
          stop(false);
      }else if (ec){
          stop(false);
      }else{
          socketOpend_ = true;
          start_read();

          heartbeat_timer_.expires_from_now(boost::posix_time::seconds(10));
          heartbeat_timer_.async_wait(boost::bind(&client::heart_beat, this));
      }
  }

  void client::start_read()
  {
      buffer_.data()[0] = '\0';
      socket_->async_read_some(boost::asio::buffer(buffer_), boost::bind(&client::handle_read, this, boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
  }

  void client::handle_read(const boost::system::error_code& error,std::size_t bytes_transferred)
  {
      if (!socketOpend_)
          return;

      if (!error)
      {
          msg_parser.parse_some(read_que_, buffer_.data(), bytes_transferred);
          start_read();
      }
      else if(error != boost::asio::error::operation_aborted)
      {
          std::cout << "error code " << error << error.message() << std::endl;
          stop(false);
      }
  }

  void client::run()
  {
      share_memory_.open_share_memory();
      share_memory_.set_memory(0,1);

      auto messageQueueThread = new std::thread(client::consume_message_queue_thread, this);
      messageQueueThread->detach();

      auto writeThread = new std::thread(client::consume_write_queue_thread, this);
      writeThread->detach();

      auto readThread = new std::thread(client::consume_read_queue_thread, this);
      readThread->detach();

      auto readThread1 = new std::thread(client::consume_read_queue_thread, this);
      readThread1->detach();

      auto ioServiceThread = new std::thread(client::run_signal_io_service_thread, this);
      ioServiceThread->detach();

      start_connect();
  }

  void client::run_signal_io_service_thread(client *clientPtr)
  {
      clientPtr->signal_io_service_.run();
  }

  void client::consume_message_queue_thread(client *clientPtr)
  {
      std::string receive_ms_name(mbus::CLIENT_MSG_QUEUE_KEY);
      message_queue::remove(receive_ms_name.c_str());
      message_queue mq(open_or_create,receive_ms_name.c_str(),10000,65535);
      while(true){
          std::string msg_str;
          unsigned int priority;
          message_queue::size_type recvd_size;
          msg_str.resize(65535);
          mq.receive(&msg_str[0], 65535, recvd_size, priority);
          msg_str.resize((int)recvd_size);
          clientPtr->add_wirte_queue(msg_str);
      }
  }

  void client::add_wirte_queue(std::string& msg)
  {
      if(ip_ == io_message::get_des_ip_from_raw(msg)){
          read_que_.push(msg);
      }else {
          if(socketOpend_){
              wirte_que_.push(msg);
          }else{
              message err;
              err.type = io_message::CLIENT_NOT_CONNECT;
              err.des_ip = io_message::get_src_ip_from_raw(msg);
              err.des_index = io_message::get_src_index_from_raw(msg);
              err.request_id = io_message::get_request_id_from_raw(msg);
              err.src_ip = err.des_ip;
              err.src_index = err.des_index;
              err.data = 'mbus client cant connect to mbus server';
              std::string errStr;
              err.encode_string(errStr);
              read_que_.push(errStr);
          }
      }
  }

  void client::consume_read_queue_thread(client *clientPtr)
  {
      while(true)
      {
          clientPtr->consume_read_queue();
      }
  }



  void client::consume_read_queue()
  {
      std::string msg;
      if(read_que_.pop(msg)) {
          int ty = io_message::get_type_from_raw(msg);
          int des_index;
          std::string processKey;
          if(ty != io_message::HEARTBEAT){
              des_index = io_message::get_des_index_from_raw(msg);
              processKey = std::string(mbus::PROCESS_MSG_QUEUE_KEY) + std::to_string(des_index);
              bool sendErr = true;
              if(share_memory_.get_memory(des_index) == 1){
                  msg_queue_ptr mq_ptr;
                  bool ret = msg_queue_manager_.find_or_add(processKey, mq_ptr);
                	if(ret){
                   		mq_ptr->mq_->send(msg.data(), msg.size(), 0);
                   		sendErr = false;
                	}
          }else{
              msg_queue_manager_.remove(processKey);
          }
          if(sendErr){
              message err;
              err.type = io_message::DESIP_NOT_EXIT;
              err.des_ip = io_message::get_src_ip_from_raw(msg);
              err.des_index = io_message::get_src_index_from_raw(msg);
              err.request_id = io_message::get_request_id_from_raw(msg);
              err.src_ip = ip_;
              err.src_index = 0;
              err.data = 'request index not opened';
              std::string errStr;
              err.encode_string(errStr);
              wirte_que_.push(errStr);
          }
        }
      }
  }

  void client::consume_write_queue_thread(client *clentPtr)
  {
      while(true)
      {
          clentPtr->consume_write_queue();
      }
  }

  void client::consume_write_queue()
  {
      std::string str;
      if(wirte_que_.pop(str)) {
          start_write(str);
      }
  }

  void client::start_write(std::string &str)
  {
      if (!socketOpend_)
          return;

      int str_len = str.length();
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
      socket_->write_some(buf, ec);

      if(ec){
          stop(false);
      }

  }

  void client::handle_write(const boost::system::error_code& ec)
  {
      if (!socketOpend_)
          return;

      if (ec)
      {
          stop(false);
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

      heartbeat_timer_.expires_from_now(boost::posix_time::seconds(30));
      heartbeat_timer_.async_wait(boost::bind(&client::heart_beat, this));
  }

  void client::check_deadline()
  {
      if (!socketOpend_)
          return;

      if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
      {
          deadline_.expires_at(boost::posix_time::pos_infin);
      }
      deadline_.async_wait(boost::bind(&client::check_deadline, this));
  }

  void client::stop(bool stopAll)
  {
     if(stopAll){
         reconnect_ = false;
      	 share_memory_.set_memory(0,0);
         signal_io_service_.stop();
         deadline_.cancel();
         heartbeat_timer_.cancel();
         //socket_->close();
      }
      socketOpend_ = false;
      io_service_.stop();
  }

  bool client::get_status()
  {
      return socketOpend_;
  }
}
