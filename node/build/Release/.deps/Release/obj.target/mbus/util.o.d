cmd_Release/obj.target/mbus/util.o := g++ '-DNODE_GYP_MODULE_NAME=mbus' '-DUSING_UV_SHARED=1' '-DUSING_V8_SHARED=1' '-DV8_DEPRECATION_WARNINGS=1' '-D_LARGEFILE_SOURCE' '-D_FILE_OFFSET_BITS=64' '-DBUILDING_NODE_EXTENSION' -I/root/.node-gyp/4.6.0/include/node -I/root/.node-gyp/4.6.0/src -I/root/.node-gyp/4.6.0/deps/uv/include -I/root/.node-gyp/4.6.0/deps/v8/include -I../include -I/usr/local/include  -fPIC -pthread -Wall -Wextra -Wno-unused-parameter -m64 -std=c++11 -O3 -ffunction-sections -fdata-sections -fno-omit-frame-pointer -fno-rtti -std=gnu++0x -MMD -MF ./Release/.deps/Release/obj.target/mbus/util.o.d.raw   -c -o Release/obj.target/mbus/util.o ../util.cc
Release/obj.target/mbus/util.o: ../util.cc ../util.h
../util.cc:
../util.h:
