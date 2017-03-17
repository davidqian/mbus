var http = require('http');
var mbus = require('mbus');
mbus.start(1);
var i = 0;
mbus.on('message',function(err, type, srcIp, index, data){
	if(err){
		console.log('error=' + err.stack.toString());
	}else{
		console.log("receive from " + data);
	}
});

var httpServer = http.createServer(function (request, response) {
	var str = 'from 1 ' + i;
	mbus.write("172.16.100.253", 2, str, 100, function(err, type, ip, index, data){
		response.writeHead(200, {'Content-Type': 'text/plain'});
        	response.end(data+'\n');
	});
	i++;
});

httpServer.on('error', function (err) {
	console.log("err");
});

httpServer.listen(9528, "0.0.0.0", function () {
	console.log("http start");
});

process.on("SIGTERM", function() {
	mbus.stop();
});
