var http = require('http');
var mbus = require('mbus');
var i = 0;
mbus.start(2);
mbus.on('message',function(err, type, srcIp, index, data){
	if(err){
		console.log('error=' + err.stack.toString());
	}else{
		mbus.write(srcIp,index,"from 2 " + i);
		i++;
	}
});

var httpServer = http.createServer(function (request, response) {
	mbus.write("172.16.100.253", 1, "from 2", 100, function(err, type, ip, index, data){
		response.writeHead(200, {'Content-Type': 'text/plain'});
        	response.end(data+'\n');
	});
});

httpServer.on('error', function (err) {
	console.log(err);
	console.log("err");
});

httpServer.listen(9529, "0.0.0.0", function () {
	console.log("http start");
});
