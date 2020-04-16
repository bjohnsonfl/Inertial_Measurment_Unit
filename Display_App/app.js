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
//const parser = usart.pipe(new byteLength({length: 16}));
parser.on('data',function(data){
	const oneG = 16384
	const twoG = oneG *2;
	//console.log(data.readInt16BE(0), data.readInt16BE(2), data.readInt16BE(4));
	//io.emit('data', {data: data});
	io.emit('data', {data: {
		xAxis : (data.readInt16BE(0) / twoG) * 100,
		yAxis : (data.readInt16BE(2) / twoG) * 100,
		zAxis : (data.readInt16BE(4) / twoG) * 100
	}});
	
	
});



 io.on('connection', (socket) => {
	console.log('a user connected');
})