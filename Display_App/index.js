
$(document).ready(function(){
	
	var socket = io();
	socket.on('data', function(req){
		console.log((100 - Math.round(Math.abs(req.data.xAxis))));

		$("#accelXraw").css({
			"height": (100 - Math.round(Math.abs(req.data.xAxis))) + "%",
			
		});

		$("#accelYraw").css({
			"height": (100 - Math.round(Math.abs(req.data.yAxis))) + "%",
		});

		$("#accelZraw").css({
			"height": (100 - Math.round(Math.abs(req.data.zAxis))) + "%",
		});

		if(req.data.xAxis < 0){
			$("#accelXRawBorder").css({
			"background": "blue",
		});
		}else{
			$("#accelXRawBorder").css({
			"background": "red",
		});}

		if(req.data.yAxis < 0){
		$("#accelYRawBorder").css({
		"background": "blue",
		});
		}else{
			$("#accelYRawBorder").css({
			"background": "red",
		});}

		if(req.data.zAxis < 0){
		$("#accelZRawBorder").css({
		"background": "blue",
		});
		}else{
			$("#accelZRawBorder").css({
			"background": "red",
		});}


	})
	

	
});





