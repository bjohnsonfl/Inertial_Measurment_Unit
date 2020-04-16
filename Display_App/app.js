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
parser.on('data', console.log);
////////////////////////////


 io.on('connection', (socket) => {
	console.log('a user connected');
})