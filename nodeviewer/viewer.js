var siteconfig = {
	name: 'viewer',
	domain: undefined,
	port: 80,
	version: "0.0.1",
}

var sys = require("sys")



///////
//  ARDUINO SERIAL
/////
var sp = require("serialport")
var arduino;

sp.list(function (err, ports) {
	/* This lists all the serial devices.. we are looking for the arduino */
	console.log("\ncomName\t\tmanufacturer")
  	console.log("====================================================================")
  	ports.forEach(function(port) {
  		var n=port.manufacturer.indexOf("arduino");
  		if (n >= 0) {
 			arduino = new sp.SerialPort(port.comName, { baudrate: 115200, parser: sp.parsers.readline("\n") } ); 				
 			arduino.on("open", arduinoConnect);
  		}
  		console.log(port.comName+"\t\t"+port.manufacturer);
    	
  	});
  	console.log("====================================================================")
});


function arduinoConnect() {
	console.log("open - connected to arduino")
	arduino.on("data", dataParser);
}

var testdata = 0;
function dataParser(data) {
	//convert datastream to string, then parse to JSON object
	var indata = JSON.parse(data.toString()); 		
	
	if (testdata == 0) {
		//only print data once.
		console.log('Only first data object will be displayed:\n')
		console.log(indata);
		testdata = 1;
	}
	io.sockets.emit("locator", indata)	
}

///////
//  WEBSERVER
/////

var http 		= require('http')				//HTTP
var	url	 		= require('url')
var path 		= require('path')
var fs 			= require('fs')					//FILESYSTEM				
var util 		= require('util')

var mimeTypes = {
    "html": "text/html",
    "jpeg": "image/jpeg",
    "jpg": "image/jpeg",
    "png": "image/png",
    "js": "text/javascript",
    "css": "text/css"};

var arrow = http.createServer(function (req, res) {
	var uri = url.parse(req.url).pathname;
	var filename = path.join(process.cwd()+"/static", uri);
	console.log(uri)
	if (uri == "/") {
			var page = path.join(process.cwd()+"/static", "/index.htm");			
			var mimeType = mimeTypes[path.extname(page).split(".")[1]]
			res.writeHead(200, {'Content-Type': mimeType})
			var fileStream = fs.createReadStream(page);
			fileStream.pipe(res);
			return;
	}
	

	//####################   STATIC FILES ################
	fs.exists(filename, function(exists) {
		if (!exists) {
			//console.log("404:"+filename);		
			res.writeHead(404, {'Content-Type': 'text/html'});
		  	res.write('error 404');
		  	res.end();
		  	return;
		} 

		var stats = fs.statSync(filename);
		//console.log("STATS: "+stats.isDirectory());

		if (stats.isDirectory()) {
			res.writeHead(200, {'Content-Type':'text/html'})
			res.write('Are you lost?')
			res.end();
		} else {
			var mimeType = mimeTypes[path.extname(filename).split(".")[1]]
			res.writeHead(200, {'Content-Type': mimeType})
			var fileStream = fs.createReadStream(filename);
			fileStream.pipe(res);	
		}		
	});	//end static server
	
})

var io = require('socket.io').listen(arrow);
io.set('log level', 1);

io.sockets.on('connection', function (socket) {
	console.log("client connected")
  socket.emit('news', { text:'socket.io', done:false });
  socket.on('my other event', function (data) {
    console.log(data);
    socket.broadcast.emit('incoming',{ textinc: data.my});
  });
  socket.on('todo', function (data) {
    console.log(data);
  });  
});

//START IT UP
console.log("Starting up server")
arrow.listen(siteconfig.port, siteconfig.domain);
if (siteconfig.domain == undefined) {
	console.log("Open your browser on localhost:"+siteconfig.port)
} else {
	console.log("Open your browser on " + siteconfig.domain + ":"+siteconfig.port)
}
