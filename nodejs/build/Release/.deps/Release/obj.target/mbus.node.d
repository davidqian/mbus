cmd_Release/obj.target/mbus.node := g++ -shared -pthread -rdynamic -m64  -Wl,-soname=mbus.node -o Release/obj.target/mbus.node -Wl,--start-group Release/obj.target/mbus/node/mbus.o Release/obj.target/mbus/node/mbusObj.o Release/obj.target/mbus/src/message/io_message.o Release/obj.target/mbus/src/message/message.o Release/obj.target/mbus/src/util/util.o Release/obj.target/mbus/src/share/share_memory.o -Wl,--end-group /usr/local/lib/libboost_locale.a /usr/local/lib/libboost_thread.a /usr/local/lib/libboost_system.a