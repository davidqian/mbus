#include "my_queue.hpp"
namespace mbus{
my_queue::my_queue() {
	
}

bool my_queue::shipment_available() {
	return !queue_.empty();
}

void my_queue::push(std::string &str) {
   pushm_.lock();
   queue_.push(str);
   std::cout << "str length = " << str.size() << std::endl;
   pushm_.unlock();
   cv_.notify_one();
}

bool my_queue::pop(std::string &str) {
   if(!queue_.empty()){
   	str = queue_.front();
   	queue_.pop();
        std::cout << "pop str length " << std::endl;
	std::cout << str.size() << std::endl;
        return true;
   }else{
	std::unique_lock<std::mutex> lk(popm_);
        cv_.wait(lk);
	return false;
   }
}
}
