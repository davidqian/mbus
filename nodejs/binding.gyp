{
  "targets": [
    {
      "target_name": "mbus",
      "sources": ["./node/mbus.cc","./node/mbusObj.cc","./src/message/io_message.cpp","./src/message/message.cpp","./src/util/util.cpp","./src/share/share_memory.cpp"],
      "include_dirs": [
            "include", "/usr/local/include","./src"
          ],
      "libraries": ["/usr/local/lib/libboost_locale.a","/usr/local/lib/libboost_thread.a","/usr/local/lib/libboost_system.a" ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags": [ "-std=c++11" ],
      "cflags_cc!": [ '-fno-rtti', "-fno-exceptions", "-fPIC" ]
    }
  ]
}
