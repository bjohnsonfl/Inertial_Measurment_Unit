var express = require("express");
var app = express();
var http = require('http').createServer(app);
var path = require('path');


//app.use("/", express.static(__dirname));
app.use(express.static(path.join(__dirname, '/')));

var port = process.env.PORT || 3000;        
var server = app.listen(port);
console.log("Listening on " + port);
var io = require('socket.io').listen(server);

//////////////////////
const serialPort = require ('serialport');

const usart = new serialPort('COM3', {baudRate : 115200 , dataBits : 8, stopBits : 1, parity : 'even'}, function(err){
	if (err){
		return console.log('Error: ', err.message);
	}
});

const Delimiter = require('@serialport/parser-delimiter');
const byteLength = require('@serialport/parser-byte-length');
const parser = usart.pipe(new Delimiter({delimiter : 'n'}))
const oneG = 16384
const twoG = oneG *2;
const LSBDegPerSec = 131;
const maxDegPerSec = 250 * LSBDegPerSec;
const tempSensitivity  = 333.87;
const tempOffset = 21;
var count = 0;		// frame rate relative to uart sample rate
//const parser = usart.pipe(new byteLength({length: 16}));
parser.on('data',function(data){
	console.log(data);
	if(count >= 25){
		if(data.length >= 14){			// check to see if uart sent every byte 
	
			io.emit('data', {data: {
				accelXraw : (data.readInt16BE(0) / twoG) * 100,
				accelYraw : (data.readInt16BE(2) / twoG) * 100,
				accelZraw : (data.readInt16BE(4) / twoG) * 100,
				gyroXraw : (data.readInt16BE(6)  / maxDegPerSec) * 100,
				gyroYraw : (data.readInt16BE(8) / maxDegPerSec) * 100,
				gyroZraw : (data.readInt16BE(10) / maxDegPerSec) * 100,
				tempRaw : (data.readInt16BE(12)),
			}});
			count = 0;	
		}
	}
	else count += 1;
});



 io.on('connection', (socket) => {
	console.log('a user connected');
})