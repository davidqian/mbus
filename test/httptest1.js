var http = require('http');

var httpServer = http.createServer(function (request, response) {
	response.writeHead(200, {'Content-Type': 'text/plain'});  
       	response.end('from 2');

});

httpServer.on('error', function (err) {
	console.log("err");
});

httpServer.listen(9529, "0.0.0.0", function () {
	console.log("http start");
});
