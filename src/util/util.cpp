//
//  mbus.cpp
//  mbus
//
//  Created by davidqian on 16/11/29.
//  Copyright (c) 2016年 davidqian. All rights reserved.
//
#include <stdio.h>      
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <iostream>
#include <sstream> 
#include "util.hpp"
namespace mbus {
	void getLocalIp(std::string& ip)
	{
		struct ifaddrs * ifAddrStruct=NULL;
    		struct ifaddrs * ifa=NULL;
    		void * tmpAddrPtr=NULL;

    		getifaddrs(&ifAddrStruct);

    		for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
    			if (!ifa->ifa_addr) {
            			continue;
        		}
        		if (ifa->ifa_addr->sa_family == AF_INET) {
       	    			tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            			char addressBuffer[INET_ADDRSTRLEN];
            			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
				if(isLAN(addressBuffer)){
					ip = addressBuffer;
					break;
				}
        		}
			/**
			 else if (ifa->ifa_addr->sa_family == AF_INET6) {
            			tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            			char addressBuffer[INET6_ADDRSTRLEN];
            			inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
				ip = addressBuffer;
        		} 
			**/
    		}
    		if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
	}

	bool isLAN(const std::string& ipstring)  
	{  
    		std::istringstream st(ipstring);  
    		int ip[2];  
    		for(int i = 0; i < 2; i++)  
    		{  
        		std::string temp;  
        		getline(st,temp,'.');  
        		std::istringstream a(temp);  
        		a >> ip[i];  
    		}  
    		if((ip[0]==10) || (ip[0]==172 && ip[1]>=16 && ip[1]<=31) || (ip[0]==192 && ip[1]==168)){
        		return true;  
		}else{
    			return false;  
		}
	}

    	int ip2long(std::string &ip)
    	{
    		std::vector<std::string> ip_elems = split(ip, ".");
    		int long_ip = 0;
    		std::vector<std::string>::iterator it;
    		int i = 3;
			for(it=ip_elems.begin();it!=ip_elems.end();it++){
				int elems = std::stoi(*it);
				long_ip = long_ip | (elems << (i * 8));
				i--;
			}
			return long_ip;
    	}

        void int2ip(int iip, std::string &ip) {
                int ip1 = (iip & (0xff << 24)) >> 24;
                ip += std::to_string(ip1);
                ip += ".";
                int ip2 = (iip & (0xff << 16)) >> 16;
                ip += std::to_string(ip2);
                ip += ".";
                int ip3 = (iip & (0xff << 8)) >> 8;
                ip += std::to_string(ip3);
                ip += ".";
                int ip4 = (iip & 0xff);
                ip += std::to_string(ip4);
        }

    	std::vector<std::string> split(const std::string& s, const std::string& delim)  
		{  
		    std::vector<std::string> elems;  
		    size_t pos = 0;  
		    size_t len = s.length();  
		    size_t delim_len = delim.length();  
		    if (delim_len == 0) return elems;  
		    while (pos < len)  
		    {  
		        int find_pos = s.find(delim, pos);  
		        if (find_pos < 0)  
		        {  
		            elems.push_back(s.substr(pos, len - pos));  
		            break;  
		        }  
		        elems.push_back(s.substr(pos, find_pos - pos));  
		        pos = find_pos + delim_len;  
		    }  
		    return elems;  
		}

	int chars2int(std::string& str, int begin, int length) {
		char chars[length];
		int i = 0;
		for(i = 0; i < length; i++){
			chars[i] = str[begin+i];
		}
		return bigBys2Uint32(chars);
	}

	int bigBys2Uint32(const char bys[4]) {
		int uint = 0;
		for(int i = 0; i <= 3; i++){
			uint |= (0xff & bys[i]) << (8*(3-i));
		}
		return uint;
	}

} // namespace mbus
