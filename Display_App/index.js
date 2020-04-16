
$(document).ready(function(){
	
	var socket = io();
	socket.on('data', function(req){
		updateAccelRaw(req);
		updateGyroRaw(req);
	})
	

	
});

function updateAccelRaw(req){
	//console.log((100 - Math.round(Math.abs(req.data.xAxis))));

	$("#accelXraw").css({
		"height": (100 - Math.round(Math.abs(req.data.accelXraw))) + "%",
			
	});

	$("#accelYraw").css({
		"height": (100 - Math.round(Math.abs(req.data.accelYraw))) + "%",
	});

	$("#accelZraw").css({
		"height": (100 - Math.round(Math.abs(req.data.accelZraw))) + "%",
	});

	if(req.data.accelXraw < 0){
		$("#accelXRawBorder").css({
		"background": "blue",
	});
	}else{
		$("#accelXRawBorder").css({
		"background": "red",
	});}

	if(req.data.accelYraw < 0){
	$("#accelYRawBorder").css({
	"background": "blue",
	});
	}else{
		$("#accelYRawBorder").css({
		"background": "red",
	});}

	if(req.data.accelZraw < 0){
	$("#accelZRawBorder").css({
	"background": "blue",
	});
	}else{
		$("#accelZRawBorder").css({
		"background": "red",
	});}
}


function updateGyroRaw(req){
	//console.log((100 - Math.round(Math.abs(req.data.xAxis))));

	$("#gyroXraw").css({
		"height": (100 - Math.round(Math.abs(req.data.gyroXraw))) + "%",
			
	});

	$("#gyroYraw").css({
		"height": (100 - Math.round(Math.abs(req.data.gyroYraw))) + "%",
	});

	$("#gyroZraw").css({
		"height": (100 - Math.round(Math.abs(req.data.gyroZraw))) + "%",
	});

	if(req.data.gyroXraw < 0){
		$("#gyroXRawBorder").css({
		"background": "blue",
	});
	}else{
		$("#gyroXRawBorder").css({
		"background": "red",
	});}

	if(req.data.gyroYraw < 0){
	$("#gyroYRawBorder").css({
	"background": "blue",
	});
	}else{
		$("#gyroYRawBorder").css({
		"background": "red",
	});}

	if(req.data.gyroZraw < 0){
	$("#gyroZRawBorder").css({
	"background": "blue",
	});
	}else{
		$("#gyroZRawBorder").css({
		"background": "red",
	});}
}