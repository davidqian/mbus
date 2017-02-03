//
//  mbusserver.cpp
//  mbus
//
//  Created by davidqian on 16/11/29.
//  Copyright (c) 2016年 davidqian. All rights reserved.
//

#include <iostream>
#include "server/server.hpp"
int main(int argc, const char * argv[]){
	mbus::server::server s("0.0.0.0", "9527");
    s.run();
}