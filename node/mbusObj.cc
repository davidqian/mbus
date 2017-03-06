#include <uv.h>
#include "mbusObj.h"
#include "message/message.hpp"
#include "message/io_message.hpp"
#include "util/util.hpp"

using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Integer;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;
using v8::Handle;
using v8::HandleScope;
using v8::CopyablePersistentTraits;

using namespace boost::interprocess;
namespace mbus {

static Persistent<Function, CopyablePersistentTraits<Function>> callback;
struct reqData
{
    mbusObj * mbusObjPtr;
    std::string result;
    bool hasErr;
};


Persistent<Function> mbusObj::constructor;

mbusObj::mbusObj():io_service_(), ip_(0), index_(0), shm_opend_(false)
{
	    auto t = new std::thread(mbusObj::singnalThread, this);
	    t->detach();
}

mbusObj::~mbusObj() {
}

void mbusObj::initObjData(int ip, int index)
{
    ip_ = ip;
    index_ = index;
    std::string key("mbus_share_memory");
    share_memory_.set_key(key);
    share_memory_.open_share_memory();
    share_memory_.set_memory(index_,1);

    std::string read_mq_name = "process_message_queue_"+std::to_string(index_);
    read_mq_ = new message_queue(open_or_create, read_mq_name.c_str(), 100, 65535);
}

bool mbusObj::checkClientWrite()
{
    if(share_memory_.get_memory(0) == 1) {
        if (!shm_opend_) {
            write_mq_ = new message_queue(open_only, "mbus_receive_message_queue");
            shm_opend_ = true;
        }
        return true;
    }else{
        return false;
    }
}

void mbusObj::stop()
{
    if(!io_service_.stopped()){
        io_service_.stop();
        io_service_.reset();
    }
}

void mbusObj::singnalThread(mbusObj * mbusObjPtr) {
	mbusObjPtr->io_service_.run();	
}

void mbusObj::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "mbusObj"));
  tpl->InstanceTemplate()->SetInternalFieldCount(3);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "write", Write);
  NODE_SET_PROTOTYPE_METHOD(tpl, "start", Start);
  NODE_SET_PROTOTYPE_METHOD(tpl, "stop", Exit);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "mbusObj"),
               tpl->GetFunction());
}

void mbusObj::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    mbusObj* obj = new mbusObj();
    obj->Wrap(args.This());
    args.GetReturnValue().Set(Number::New(isolate, 1));
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Context> context = isolate->GetCurrentContext();
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Object> result =
        cons->NewInstance(context, argc, argv).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}

void mbusObj::Start(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    mbusObj* obj = ObjectWrap::Unwrap<mbusObj>(args.Holder());

    v8::String::Utf8Value ip_v8(args[0]->ToString());
    std::string ip = std::string(*ip_v8);
    obj->initObjData(ip2long(ip), args[1]->NumberValue());

    Handle<Function> cb_ = Handle<Function>::Cast(args[2]);
    Persistent<Function> cb(isolate, cb_);
    callback = cb;

    reqData* request = new reqData;
    request->mbusObjPtr = obj;
    request->hasErr = false;
    uv_work_t* req = new uv_work_t();
    req->data = request; 

    uv_queue_work(uv_default_loop(), req, mbusObj::onWork, mbusObj::afterWork);

    args.GetReturnValue().Set(Number::New(isolate, 1));
}

void mbusObj::onWork(uv_work_t* req) {
    reqData* request = (reqData*)req->data;
    try{
      unsigned int priority;
      message_queue::size_type recvd_size;
      std::string msg_str;
      msg_str.resize(65535);
      request->mbusObjPtr->read_mq_->receive(&msg_str[0], 65535, recvd_size, priority);
      msg_str.resize((int)recvd_size);
      request->result = msg_str;
    }catch(interprocess_exception &ex){
        request->hasErr = true;
        request->result = ex.what();
    }
}

void mbusObj::afterWork(uv_work_t* req, int status) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    reqData* request = (reqData*)req->data;
    delete req;

    std::string ip;
    std::string data;
    int src_index;
    int request_id;
    int type;

    if(!request->hasErr) {
        type = io_message::get_type_from_raw(request->result);

        int src_ip = io_message::get_src_ip_from_raw(request->result);
        int2ip(src_ip, ip);

        src_index = io_message::get_src_index_from_raw(request->result);
        request_id = io_message::get_request_id_from_raw(request->result);
        io_message::get_data_from_raw(request->result, data);

    }else{
        type = io_message::UNKONW_ERROR;
        ip = "127.0.0.1";
        src_index = 0;
        request_id = 0;
        data = request->result;
    }

    const unsigned argc = 5;
    Local <Value> argv[argc] = {Number::New(isolate, type),Number::New(isolate, request_id), String::NewFromUtf8(isolate, ip.data()),
                                    Number::New(isolate, src_index), String::NewFromUtf8(isolate, data.data())};

    auto fn = Local<Function>::New(isolate, callback);
    fn->Call(isolate->GetCurrentContext()->Global(), argc, argv);

    uv_work_t* req1 = new uv_work_t();
    req1->data = request;
    uv_queue_work(uv_default_loop(), req1, mbusObj::onWork, mbusObj::afterWork);
}

void mbusObj::Exit(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  mbusObj* obj = ObjectWrap::Unwrap<mbusObj>(args.Holder());
  obj->stop();
}

void mbusObj::Write(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  mbusObj* obj = ObjectWrap::Unwrap<mbusObj>(args.Holder());

  if(obj->checkClientWrite()){
     v8::String::Utf8Value des_ip_v8(args[0]->ToString());
     std::string des_ip = std::string(*des_ip_v8);
     v8::String::Utf8Value param(args[3]->ToString());

     message msg;
     msg.type = io_message::REQUEST;
     msg.des_ip = ip2long(des_ip);
     msg.des_index = args[1]->NumberValue();
     msg.src_ip = obj->ip_;
     msg.src_index = obj->index_;
     msg.request_id = args[2]->NumberValue();
     msg.data = std::string(*param);
     std::string m;
     msg.encode_string(m); 

     obj->write_mq_->send(m.data(), m.size(), 0);
     args.GetReturnValue().Set(Number::New(isolate, 1));
  }else{
     obj->shm_opend_ = false;
     args.GetReturnValue().Set(Number::New(isolate, 0));
  }
}
}  // namespace demo
