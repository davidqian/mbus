//
// Created by playcrab on 1/20/17.
//

#include "share_memory.hpp"

namespace mbus {
	share_memory::share_memory(const std::string& key):key_(key),shm_opened_(false){}

	share_memory::share_memory():key_(""),shm_opened_(false){}

	share_memory::~share_memory(){
		delete mmap_;
		delete shm_;
	}

	void share_memory::set_key(const std::string& key) {
		key_ = key;
	}

	void share_memory::open_share_memory() {
		shm_ = new shared_memory_object(open_or_create, key_.c_str(), read_write);
		shm_->truncate (10000);
		mmap_ = new mapped_region(*shm_, read_write);
    shm_opened_ = true;
	}

	void share_memory::set_memory(int index, int data) {
		if(shm_opened_){
			std::memset(mmap_->get_address()+index, data, 1);
		}
	}

	int share_memory::get_memory(int index) {
		if(shm_opened_){
			char *mem = static_cast<char*>(mmap_->get_address());
			return (int)(*(mem+=index));
		}else{
			return 0;
		}
	}
}
