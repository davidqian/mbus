// mbusObj.h
#ifndef MBUSOBJECT_H
#define MBUSOBJECT_H

#include <node.h>
#include <uv.h>
#include <node_object_wrap.h>
#include <thread>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include "share/share_memory.hpp"

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
  void initObjData(int ip, int index);
  void startReadMq();
  void stop();
  bool checkClientWrite();
  static void singnalThread(mbusObj * mbusObjPtr);

 private:
    explicit mbusObj();
    ~mbusObj();
  
  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Write(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Exit(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Start(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void GetIp(const v8::FunctionCallbackInfo<v8::Value>& args);

  static void onWork(uv_work_t* req);
  static void afterWork(uv_work_t* req, int status);

  static v8::Persistent<v8::Function> constructor;

  private:
    boost::asio::io_service io_service_;
    int ip_;
    int index_;

    message_queue* write_mq_;
    message_queue* read_mq_;
    share_memory share_memory_;

    bool shm_opend_;
};

}  // namespace demo

#endif
