var http = require('http');
const mbus = require('./build/Release/mbus');

process.on('exit',function(){
	console.log("here");
});

var received = 0;
var obj = new mbus.mbusObj();
obj.start("127.0.0.1",1,function(type, requestId, ip, index, data){
	received++;
	console.log(received);
});

var httpServer = http.createServer(function (request, response) {
	obj.write("127.0.0.1", 1, 1, "from 1");

	response.writeHead(200, {'Content-Type': 'text/plain'});  
        response.end('ok\n');
});

httpServer.on('error', function (err) {
	console.log("err");
});

httpServer.listen(9528, "0.0.0.0", function () {
	console.log("http start");
});

