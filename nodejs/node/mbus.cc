#include <node.h>
#include "mbusObj.h"

namespace mbus {

using v8::Local;
using v8::Object;

void InitAll(Local<Object> exports) {
  mbusObj::Init(exports);
}

NODE_MODULE(mbus, InitAll)

}
