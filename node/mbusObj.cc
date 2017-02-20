#include <uv.h>
#include "mbusObj.h"
#include "message.h"
#include "io_message.h"
#include "util.h"
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
};


Persistent<Function> mbusObj::constructor;

mbusObj::mbusObj():ip_(0),index_(0){
}

mbusObj::~mbusObj() {
}

void mbusObj::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "mbusObj"));
  tpl->InstanceTemplate()->SetInternalFieldCount(2);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "write", Write);
  NODE_SET_PROTOTYPE_METHOD(tpl, "start", Start); 

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
    // Invoked as plain function `MyObject(...)`, turn into construct call.
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
    obj->ip_ = ip2long(ip);

    obj->index_ = args[1]->NumberValue();;
    
    Handle<Function> cb_ = Handle<Function>::Cast(args[2]);
    Persistent<Function> cb(isolate, cb_);
    callback = cb;

    obj->write_mq_ = new message_queue(open_only, "mbus_receive_message_queue");

    std::string read_mq_name = "process_message_queue_"+std::to_string(obj->index_);
    message_queue::remove(read_mq_name.c_str());
    obj->read_mq_ = new message_queue(open_or_create, read_mq_name.c_str(), 10, 65535);

    reqData* request = new reqData; 
    request->mbusObjPtr = obj;
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
        std::cout << ex.what() << std::endl;
    }     
}

void mbusObj::afterWork(uv_work_t* req, int status) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    reqData* request = (reqData*)req->data;
    delete req;	 	

    int src_ip = io_message::get_src_ip_from_raw(request->result);
    std::string ip = "";
    int2ip(src_ip, ip);

    int src_index = io_message::get_src_index_from_raw(request->result);
    int request_id = io_message::get_request_id_from_raw(request->result);

    std::string data;
    io_message::get_data_from_raw(request->result, data);

    const unsigned argc = 4;
    Local<Value> argv[argc] = {Number::New(isolate,request_id),String::NewFromUtf8(isolate,ip.data()), Number::New(isolate,src_index), String::NewFromUtf8(isolate,data.data()) };
    
    auto fn = Local<Function>::New(isolate, callback);
    fn->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    uv_work_t* req1 = new uv_work_t();
    req1->data = request;
    uv_queue_work(uv_default_loop(), req1, mbusObj::onWork, mbusObj::afterWork);
}

void mbusObj::Write(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  v8::String::Utf8Value des_ip_v8(args[0]->ToString());
  std::string des_ip = std::string(*des_ip_v8);
  v8::String::Utf8Value param(args[3]->ToString());

  mbusObj* obj = ObjectWrap::Unwrap<mbusObj>(args.Holder());

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
}
}  // namespace demo
