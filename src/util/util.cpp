//
//  mbus.cpp
//  mbus
//
//  Created by davidqian on 16/11/29.
//  Copyright (c) 2016年 davidqian. All rights reserved.
//
#include "util.hpp"
namespace mbus {
    namespace util {

    	int ip2long(std::string ip)
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

    } // namespace util
} // namespace mbus
