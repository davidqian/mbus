//
// Created by playcrab on 1/20/17.
//

#ifndef MBUS_SHARE_MEMORY_HPP
#define MBUS_SHARE_MEMORY_HPP
#include <boost/interprocess/managed_shared_memory.hpp>  
#include <boost/interprocess/mapped_region.hpp>
#include <cstring>
#include <cstdlib>
#include <string>
using namespace boost::interprocess;

namespace mbus {
        class share_memory
        {
        public:
            share_memory(const share_memory&) = delete;
            share_memory& operator=(const share_memory&) = delete;

            share_memory(const std::string& key);
	    share_memory();
	    ~share_memory();
	    void set_key(const std::string& key);
	    void open_share_memory();
	    void set_memory(int index, int data);
	    int get_memory(int index);
        public:
            shared_memory_object* shm_;
	    mapped_region* mmap_;
	private:
	    std::string key_;
	    bool shm_opened_;
        };
}
#endif //MBUS_MSG_QUEUE_HPP
