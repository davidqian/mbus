{
  "targets": [
    {
      "target_name": "mbus",
      "sources": ["mbus.cc","mbusObj.cc","io_message.cc","message.cc","util.cc" ],
      "include_dirs": [
            "include", "/usr/local/include"
          ],
      "libraries": ["/usr/local/lib/libboost_locale.a","/usr/local/lib/libboost_thread.a","/usr/local/lib/libboost_system.a" ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags": [ "-std=c++11" ],
      "cflags_cc!": [ "-fno-exceptions", "-fPIC" ]
    }
  ]
}
