
$(document).ready(function(){
	
	var socket = io();
	socket.on('data', function(req){
		updateAccelRaw(req);
		updateGyroRaw(req);
		updateTemp(req);
	})
	

	
});

function updateAccelRaw(req){
	//console.log((100 - Math.round(Math.abs(req.data.xAxis))));

	$("#accelXraw").css({
		"height": (100 - Math.round(Math.abs(req.data.accelXraw))) + "%",
	});
	$("#accelXRawVal").html(Math.round(req.data.accelXraw / 50 * 1000)  + " mG");
	
	$("#accelYraw").css({
		"height": (100 - Math.round(Math.abs(req.data.accelYraw))) + "%",
	});
	$("#accelYRawVal").html(Math.round(req.data.accelYraw / 50 * 1000)  + " mG");

	$("#accelZraw").css({
		"height": (100 - Math.round(Math.abs(req.data.accelZraw))) + "%",
	});
	$("#accelZRawVal").html(Math.round(req.data.accelZraw / 50 * 1000)  + " mG");

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
	$("#gyroXRawVal").html(Math.round(req.data.gyroXraw / 100 * 250)  + " deg/sec");
	

	$("#gyroYraw").css({
		"height": (100 - Math.round(Math.abs(req.data.gyroYraw))) + "%",
	});
	$("#gyroYRawVal").html(Math.round(req.data.gyroYraw / 100 * 250)  + " deg/sec");

	$("#gyroZraw").css({
		"height": (100 - Math.round(Math.abs(req.data.gyroZraw))) + "%",
	});
	$("#gyroZRawVal").html(Math.round(req.data.gyroZraw / 100 * 250)  + " deg/sec");

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

const tempSensitivity  = 333.87;
const tempOffset = 21;
const celsiusRange = 50;


function updateTemp(req){
	var celsius = req.data.tempRaw / tempSensitivity + tempOffset;
	var fahrenheit = celsius * (9/5) + 32;
	var celsiusPercent = Math.round(Math.abs(((celsius / celsiusRange) * 100)));
	celsius = Math.round(celsius * 1000) / 1000;
	fahrenheit = Math.round(fahrenheit * 1000) / 1000;
	

	$("#CelsiusVal").html(celsius + " C");
	$("#FahrenheitVal").html(fahrenheit + " F");

	$("#tempRaw").css({
		"height": celsiusPercent + "%",
	});

	if(celsius < 0){
	$("#tempRawBorder").css({
	"background": "blue",
	});
	}else{
		$("#tempRawBorder").css({
		"background": "red",
	});}
}