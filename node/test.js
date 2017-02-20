var http = require('http');
const mbus = require('./build/Release/mbus');

var obj = new mbus.mbusObj();
console.log("here 1");
obj.start("127.0.0.1",1,function(requestId, ip, index, data){
	console.log("requestId=" + requestId);
	console.log("ip=" + ip);
	console.log("index=" + index);
	console.log("data=" + data);
});
console.log("here 2");
var httpServer = http.createServer(function (request, response) {
	obj.write("127.0.0.1", 1, 1, "abc");
});
console.log("here 3");

httpServer.on('error', function (err) {
	console.log("err");
});

console.log("here 4");
httpServer.listen(9528, "0.0.0.0", function () {
	console.log("http start");
});
console.log("here 5");
