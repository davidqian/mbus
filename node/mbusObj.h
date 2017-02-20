// mbusObj.h
#ifndef MBUSOBJECT_H
#define MBUSOBJECT_H

#include <node.h>
#include <uv.h>
#include <node_object_wrap.h>
#include <thread>
#include <boost/interprocess/ipc/message_queue.hpp>
using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;
using v8::CopyablePersistentTraits;
using namespace boost::interprocess;
namespace mbus {

class mbusObj : public node::ObjectWrap {

 public:
  static void Init(v8::Local<v8::Object> exports);
  int ip_;
  int index_;

 private:
  explicit mbusObj();
  ~mbusObj();
  //static void consume_read_queue_thread(void *arg); 
  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Write(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Start(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void onWork(uv_work_t* req);
  static void afterWork(uv_work_t* req, int status);
  static void asyncCallback(uv_async_t *handle);
  static v8::Persistent<v8::Function> constructor;

  private:
   message_queue* write_mq_;
   message_queue* read_mq_;
};

}  // namespace demo

#endif
