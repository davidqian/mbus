//
// Created by playcrab on 1/13/17.
//
#include "client/client.hpp"
int main(int argc, const char * argv[]){
        mbus::client c("127.0.0.1", "9527", "mbus_share_memory");
        c.run();
        return 0;
}

