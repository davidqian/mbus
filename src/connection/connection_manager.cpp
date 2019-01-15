//
// connection_manager.cpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
//
#include <map>
#include "connection/connection_manager.hpp"
#include "message/message_parser.hpp"
#include "util/util.hpp"

namespace mbus {

        connection_manager::connection_manager()
        {
        }

        void connection_manager::add(connection_ptr c)
        {
            int remote_ip;
            try{
                remote_ip = c->get_remote_ip();
                connections_[remote_ip] = c;
                c->start();
            }catch (std::exception& e){
                c->stop();
            }
        }

        void connection_manager::find(int ip, connection_ptr &conn)
        {
            conn = nullptr;
            std::map<int, connection_ptr>::iterator iter;
            iter = connections_.find(ip);
            if(iter != connections_.end()){
                conn = iter->second;
            }
        }

        void connection_manager::stop(connection_ptr c)
        {
            int remote_ip;
            try{
		            remote_ip = c->get_remote_ip();
                connections_.erase(remote_ip);
            }catch (std::exception& e){
            	std::cout << "connection manager stop " << e.what() << std::endl;
            }
            c->stop();
        }

        void connection_manager::stop_all()
        {
            std::map<int,connection_ptr>::iterator it;
            for(it=connections_.begin(); it!=connections_.end(); ++it) {
                it->second->stop();
            }
            connections_.clear();
        }

        void connection_manager::check_heart_beat()
        {
            time_t now = time(nullptr);
            std::map<int,connection_ptr>::iterator it;
            for(it=connections_.begin(); it!=connections_.end(); ++it) {
                if(now - it->second->heartbeat_time_ > 180){
                    it->second->stop();
                }
            }
        }

        void connection_manager::consume_msg()
        {
            std::string str;
            if(msg_que_.pop(str)) {
		            message msg;
                message_parser::parse_string(str, msg);
                connection_ptr src_ptr;
                find(msg.src_ip, src_ptr);

                if(!src_ptr){
                    return;
                }

                if(msg.type == io_message::HEARTBEAT){
                    src_ptr->heartbeat_time_ = time(nullptr);
                    src_ptr->do_write(str);
                    return;
                }

                connection_ptr des_ptr;
                find(msg.des_ip, des_ptr);
                if(des_ptr){
                    des_ptr->do_write(str);
                }else{
                    message not_exit_msg;
                    not_exit_msg.type =io_message::DESIP_NOT_EXIT;
                    not_exit_msg.des_ip = msg.des_ip;
                    not_exit_msg.des_index = msg.des_index;
                    not_exit_msg.src_ip = msg.src_ip;
                    not_exit_msg.src_index = msg.src_index;
                    not_exit_msg.request_id = msg.request_id;
		                std::string err_str;
		                not_exit_msg.encode_string(err_str);
                    src_ptr->do_write(err_str);
                }
            }
        }

} // namespace mbus
