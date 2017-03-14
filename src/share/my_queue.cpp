#include "my_queue.hpp"
namespace mbus{
    my_queue::my_queue():max_queue_len_(1000){}

    void my_queue::push(std::string &str)
    {
       std::lock_guard<std::mutex> lk(m_);
       queue_.push(str);
       cv_.notify_all();
    }

    bool my_queue::pop(std::string &str) {
       std::unique_lock<std::mutex> lk(m_);
       cv_.wait(lk,[this]{ !queue_.empty();});
       str = queue_.front();
       queue_.pop();
       return true;
   }
}
