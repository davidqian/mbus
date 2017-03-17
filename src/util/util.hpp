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
#include <vector>
namespace mbus {
  const std::string CLIENT_MSG_QUEUE_KEY = "mbus_receive_message_queue";
  const std::string PROCESS_MSG_QUEUE_KEY = "process_message_queue_";
  const std::string MBUS_SHARE_MEMORY_KEY = "mbus_share_memory";

  int ip2long(std::string &ip);
  void int2ip(int iip, std::string &ip);
  std::vector<std::string> split(const std::string& s, const std::string& delim);
	int chars2int(std::string& str, int begin, int length);
	int bigBys2Uint32(const char bys[4]);
	void getLocalIp(std::string& ip);
	bool isLAN(const std::string& ipstring);
} // namespace mbus

#endif // MBUS_UTIL_HPP
