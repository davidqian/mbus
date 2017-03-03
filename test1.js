var http = require('http');
const mbus = require('./build/Release/mbus');

process.on('exit',function(){
	console.log("here");
});

var obj = new mbus.mbusObj();
obj.start("127.0.0.1",2,function(type, requestId, ip, index, data){
	console.log("====================");
	console.log("requestId=" + requestId);
	console.log("ip=" + ip);
	console.log("index=" + index);
	console.log("data=" + data);
	console.log("====================");
});
var httpServer = http.createServer(function (request, response) {
	obj.write("127.0.0.1", 1, 1, "from 2");

	response.writeHead(200, {'Content-Type': 'text/plain'});  
        response.end('ok\n');
});

httpServer.on('error', function (err) {
	console.log("err");
});

httpServer.listen(9529, "0.0.0.0", function () {
	console.log("http start");
});
