cmake_minimum_required (VERSION 2.8)
project (./mbus)

# Select flags.
SET(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
SET(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

# The version number.
set (mbus_VERSION_MAJOR 1)
set (mbus_VERSION_MINOR 0)

include_directories (".")
include_directories ("./src")

include_directories ("./third_party/boost_1_57_0")

set (BOOST_LIB_DIR "${PROJECT_SOURCE_DIR}/./third_party/boost_1_57_0/stage/lib")

ADD_DEFINITIONS(
${CMAKE_CXX_FLAGS_DEBUG}
#${CMAKE_CXX_FLAGS_RELEASE}
# Other flags
)

add_executable (mbusd
        ./src/mbus.cpp
        )

if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
    MESSAGE("Mac platform")
    #please open below option only on mac platform else the luaJIT will crash
    SET(CMAKE_EXE_LINKER_FLAGS "-pagezero_size 10000 -image_base 100000000")
else()
    MESSAGE("Linux platform")
    #please open below option only on linux platform else the thread and dlopen will not be correct linked
    SET(CMAKE_EXE_LINKER_FLAGS "-lpthread -ldl -lrt")
endif()

target_link_libraries (mbusd
        ${BOOST_LIB_DIR}/libboost_locale.a
        ${BOOST_LIB_DIR}/libboost_thread.a
        ${BOOST_LIB_DIR}/libboost_system.a
        )
