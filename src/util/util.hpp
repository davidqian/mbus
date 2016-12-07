//
// message.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MBUS_UTIL_HPP
#define MBUS_UTIL_HPP
#include <string>

namespace mbus {
    namespace util {
    	int ip2long(std::string ip);
    	std::vector<std::string> split(const std::string& s, const std::string& delim);
    } // namespace util
} // namespace mbus

#endif // MBUS_UTIL_HPP