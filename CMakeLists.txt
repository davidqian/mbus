cmake_minimum_required (VERSION 2.8)
project (mbus)

# Select flags.
SET(CMAKE_CXX_FLAGS "-std=c++11 -Wall -lstdc++")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

include_directories (".")
include_directories ("./src")

include_directories ("./third_party/boost_1_57_0")

set (BOOST_LIB_DIR "${PROJECT_SOURCE_DIR}/./third_party/boost_1_57_0/stage/lib")

ADD_DEFINITIONS(
#${CMAKE_CXX_FLAGS_DEBUG}
${CMAKE_CXX_FLAGS_RELEASE}
# Other flags
)

add_executable (mbusc
        ./src/client.cpp
        ./src/client/client.cpp
        ./src/message/message.cpp
        ./src/message/message_parser.cpp
	./src/message/io_message.cpp
	./src/share/my_queue.cpp
        ./src/share/msg_queue.cpp
        ./src/share/msg_queue_manager.cpp
	./src/share/share_memory.cpp
        ./src/util/util.cpp
        )

#SET(CMAKE_MODULE_LINKER_FLAGS "-pagezero_size 10000 -image_base 100000000")
if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
    MESSAGE("Mac platform")
    #please open below option only on mac platform else the luaJIT will crash
    SET(CMAKE_EXE_LINKER_FLAGS "-pagezero_size 10000 -image_base 100000000")
else()
    MESSAGE("Linux platform")
    #please open below option only on linux platform else the thread and dlopen will not be correct linked
    SET(CMAKE_EXE_LINKER_FLAGS "-lpthread -ldl -lrt")
endif()

target_link_libraries (mbusc
        ${BOOST_LIB_DIR}/libboost_locale.a
        ${BOOST_LIB_DIR}/libboost_thread.a
        ${BOOST_LIB_DIR}/libboost_system.a
        )



add_executable (mbusd
        ./src/server.cpp
        ./src/server/server.cpp
        ./src/connection/connection.cpp
        ./src/connection/connection_manager.cpp
        ./src/message/message.cpp
        ./src/message/message_parser.cpp
        ./src/message/io_message.cpp
        ./src/share/my_queue.cpp
        ./src/share/msg_queue.cpp
        ./src/share/msg_queue_manager.cpp
        ./src/util/util.cpp
        )

target_link_libraries(mbusd
        ${BOOST_LIB_DIR}/libboost_locale.a
        ${BOOST_LIB_DIR}/libboost_thread.a
        ${BOOST_LIB_DIR}/libboost_system.a
        )

add_executable (test
        ./src/test.cpp
        ./src/test/test.cpp
        ./src/message/message.cpp
        ./src/message/message_parser.cpp
        ./src/message/io_message.cpp
        ./src/share/my_queue.cpp
        ./src/share/msg_queue.cpp
        ./src/share/msg_queue_manager.cpp
        ./src/util/util.cpp
        )

target_link_libraries(test
        ${BOOST_LIB_DIR}/libboost_locale.a
        ${BOOST_LIB_DIR}/libboost_thread.a
        ${BOOST_LIB_DIR}/libboost_system.a
        )
