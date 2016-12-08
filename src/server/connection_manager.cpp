//
// connection_manager.cpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <map>
#include "connection_manager.hpp"
#include "../util/util.hpp"

namespace mbus {
    namespace server {

        connection_manager::connection_manager()
        {
        }

        void connection_manager::start(connection_ptr c)
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

    } // namespace server
} // namespace mbus
