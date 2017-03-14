var http = require('http');

var httpServer = http.createServer(function (request, response) {
        http.get("http://localhost:9529/", function(res) {  
  	res.on('data', function(data) {  
		response.writeHead(200, {'Content-Type': 'text/plain'});  
       		response.end(data+'\n');
  	});  
	}).on('error', function(e) {  
		response.writeHead(200, {'Content-Type': 'text/plain'});  
      		response.end('err\n');
	});  	

});

httpServer.on('error', function (err) {
	console.log("err");
});

httpServer.listen(9528, "0.0.0.0", function () {
	console.log("http start");
});
