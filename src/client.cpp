//
// Created by playcrab on 1/13/17.
//
#include "client/client.hpp"
#include "util/util.hpp"
int main(int argc, const char * argv[]){
	std::string ipStr;
	mbus::getLocalIp(ipStr);
	int ip = mbus::ip2long(ipStr);
        mbus::client c("127.0.0.1", "9527", ip, "mbus_share_memory");
        c.run();
        return 0;
}

