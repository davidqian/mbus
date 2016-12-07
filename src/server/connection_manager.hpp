//
// connection_manager.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MBUS_CONNECTION_MANAGER_HPP
#define MBUS_CONNECTION_MANAGER_HPP

#include <map>
#include "connection.hpp"

namespace mbus {
    namespace server {

/// Manages open connections so that they may be cleanly stopped when the server
/// needs to shut down.
        class connection_manager
        {
        public:
            connection_manager(const connection_manager&) = delete;
            connection_manager& operator=(const connection_manager&) = delete;

            /// Construct a connection manager.
            connection_manager();

            /// Add the specified connection to the manager and start it.
            void start(connection_ptr c);

            /// Find the specified connection with ip

            connection_ptr find(int ip);

            /// Stop the specified connection.
            void stop(connection_ptr c);

            /// Stop all connections.
            void stop_all();

        private:
            /// The managed connections.
            std::map<int, connection_ptr> connections_;
        };

    } // namespace server
} // namespace mbus

#endif // MBUS_CONNECTION_MANAGER_HPP