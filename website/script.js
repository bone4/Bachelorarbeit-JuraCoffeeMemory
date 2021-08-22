var offline = false;
var lastEEPROMdump = '';
var lastRAMdump = '';

var valueTrue = '<span class="badge badge-success">on</span>';
var valueFalse = '<span class="badge badge-danger">off</span>';

function handleIntroJS(e) {
	$('.introjs-overlay, .introjs-helpLayer, .introjs-tooltipReferenceLayer, .introjs-fixedTooltip').appendTo('body');
	// enable bottom right buttons
	$('#commandButton').removeClass("disabled");
	$('#loadRAMbutton').removeClass("disabled");
	$('#helperButtons').css("position", "absolute");
	// show Command button instead of Send box
	$('#commandButton').show();
	$('#commandBox').hide();
	
	switch ($(e).attr('data-step')) {
	case "1": // console.log("step1");
		$('#myModal').modal('hide');
		break;
	case "2": // console.log("step2");
		$('#myModal').modal('hide');
		// un-show website
		document.getElementById("content").style.display = "none";
		document.getElementById("connectionMSG").style.display = "block";
		break;
	case "3": // console.log("step3");
		$('#myModal').modal('hide');
		// show website
		document.getElementById("content").style.display = "block";
		document.getElementById("connectionMSG").style.display = "none";
		// and show example values
		offline = true;
		start();
		break;
	case "4": // console.log("step4");
		$('#myModal').modal('hide');
		break;
	case "5": // console.log("step5");
		$('.introjs-overlay, .introjs-helpLayer, .introjs-tooltipReferenceLayer, .introjs-fixedTooltip').appendTo('#myModal');
		changeEEPROM($('#automatic_switch_off_interval'));
		break;
	case "6": // console.log("step6");
		$('.introjs-overlay, .introjs-helpLayer, .introjs-tooltipReferenceLayer, .introjs-fixedTooltip').appendTo('#myModal');
		changeEEPROM($('#automatic_switch_off_interval'));
		break;
	case "7": // console.log("step7");
		$('.introjs-overlay, .introjs-helpLayer, .introjs-tooltipReferenceLayer, .introjs-fixedTooltip').appendTo('#myModal');
		changeEEPROM($('#automatic_switch_off_interval'));
		break;
	case "8": // console.log("step8");
		$('.introjs-overlay, .introjs-helpLayer, .introjs-tooltipReferenceLayer, .introjs-fixedTooltip').appendTo('#myModal');
		changeEEPROM($('#automatic_switch_off_interval'));
		break;
	case "9": // console.log("step9");
		$('.introjs-overlay, .introjs-helpLayer, .introjs-tooltipReferenceLayer, .introjs-fixedTooltip').appendTo('#myModal');
		changeEEPROM($('#automatic_switch_off_interval'));
		break;
	case "10": // console.log("step10");
		$('#myModal').modal('hide');
		break;
	case "11": // console.log("step11");
		$('#myModal').modal('hide');
		break;
	case "12": // console.log("step12");
		$('#myModal').modal('hide');
		break;
	case "13": // console.log("step13");
		$('#myModal').modal('hide');
		break;
	}
}

function start() {
	$("#loading").show();
	if (!offline) {
		$.ajax({
			url: "data.php?getEEPROM",
			dataType: 'json',
			success: function(data) { updateEEPROM(data); }
		})
		 .fail(function() { console.error( "error getting EEPROM data" ); });
		// $("#loading").hide(); // in loadRAM()
	} else {
		$.getJSON( "offline_eeprom.json", function( data ){ updateEEPROM(data); })
		 .fail(function() { console.error( "error getting EEPROM data" ); });
		// $("#loading").hide(); // in loadRAM()
	}
}

function loadRAM() {
	$("#loading").show();
	if (!offline) {
		$.ajax({
			url: "data.php?getRAM",
			dataType: 'json',
			success: function(data) { updateRAM(data); }
		})
		 .fail(function() { console.error( "error getting RAM data" ); })
		 .always(function() { $("#loading").hide(); });
	} else {
		$.getJSON( "offline_ram.json", function( data ){ updateRAM(data); })
		 .fail(function() { console.error( "error getting RAM data" ); })
		 .always(function() { $("#loading").hide(); });
	}
}

function updateEEPROM(data) {
	if (data["error"] != undefined) {
		console.error("Error updateEEPROM(), error number: "+data["error"]["number"]+", error msg: "+data["error"]["msg"]);
		$("#loading").hide();
	} else {
		lastEEPROMdump = data;
		loadRAM();
	}

	$.each( data, function( key, dataVal ) {
		var val = dataVal["value"];

		$("#"+key+"-txt").html( $("#"+key).attr('txt') ); // set labels
		//label: "value", "valid_values", "default", "disabled", "min", "max", undefined...

		if ((key == "amount_ground_1") || // progress bar
			(key == "amount_ground_2") ||
			(key == "amount_coffee_preparations_until_next_cleaning") ||
			(key == "amount_rinsings_until_next_cleaning") ||
			(key == "amount_water") ||
			(key == "amount_calcified")) {
			
			var percentage = Math.round( (val-dataVal["min"]) / (dataVal["max"]-dataVal["min"]) * 100);
			$("#"+key+"-bar").attr("style", "width: "+percentage+"%;  cursor: pointer;");
			$("#"+key+"-bar").attr("aria-valuenow", val);
			$("#"+key+"-bar").attr("aria-valuemin", dataVal["min"]);
			$("#"+key+"-bar").attr("aria-valuemax", dataVal["max"]);
			$("#"+key+"-bar").text(percentage+"%");
			if (val > dataVal["max"]) {
				$("#"+key+"-bar").addClass("bg-danger");
			} else {
				$("#"+key+"-bar").removeClass("bg-danger");
			}
		} else { // or normal values
			$("#"+key).html(val);
			if (key == "temperature") {
				$('#temperatureA').text(val);
				$('#temperatureB').text(val);
			}
		}

	});
}
function updateRAM(data) {
	if (data["error"] != undefined) {
		console.error("Error updateRAM(), error number: "+data["error"]["number"]+", error msg: "+data["error"]["msg"]);
	} else {
		lastRAMdump = data;

		// show website
		document.getElementById("content").style.display = "block";
		document.getElementById("connectionMSG").style.display = "none";

		// enable bottom right buttons
		$('#commandButton').removeClass("disabled");
		$('#loadRAMbutton').removeClass("disabled");
		
	}

	$.each( data, function( key, val ) {
		$("#"+key+"-txt").html( $("#"+key).attr('txt') ); // set labels
		
		// set values
		if (typeof val == "boolean") {
			if (val == true) {
				$("#"+key).html( valueTrue );
			} else {
				$("#"+key).html( valueFalse );
			}
		} else {
			$("#"+key).html(val);
		}
	});

	// illustration
	if (data["fill_beans"] || data["fill_powder"])  {
		$("#animation_beans").show();
	} else {
		$("#animation_beans").hide();
	}
	if (data["fill_water"] || data["replacing_filter"])  {
		$("#animation_water").show();
	} else {
		$("#animation_water").hide();
	}
	if (data["empty_drip_tray"] || data["empty_grounds"] || data["tray_missing"]) {
		$("#animation_drip-tray").show();
	} else {
		$("#animation_drip-tray").hide();
	}
}

function setNewValue(elem) {
	$('#myModalNewValue').val( $(elem).attr("txt") );
}

function changeEEPROM(elem) {
	if (($(elem).attr('id') == "temperatureA") || ($(elem).attr('id') == "temperatureB")) {
		elem = $('#temperature');
	}
	//console.log($(elem));
	var key = $(elem).attr('id').replace("-bar",""); // xxx-bar => xxx
	$('#myModalKey').val( key );
	$('#myModalTitle').html( $(elem).attr('txt') );
	if ((key == "amount_ground_1") || // progress bar
		(key == "amount_ground_2") ||
		(key == "amount_coffee_preparations_until_next_cleaning") ||
		(key == "amount_rinsings_until_next_cleaning") ||
		(key == "amount_water") ||
		(key == "amount_calcified")) {
		
		$('#myModalOldValue').val( $(elem).attr("aria-valuenow") );
	} else { // or normal values
		$('#myModalOldValue').val( $(elem).text() );
	}
	$('#myModalNewValue').removeClass("is-invalid");
	$('#myModalNewValue').val( "" );
	if (lastEEPROMdump[key]["min"] == undefined) {
		$('#myModalNewValue').attr("min", 0);
	} else {
		$('#myModalNewValue').attr("min", lastEEPROMdump[key]["min"]);
	}
	if (lastEEPROMdump[key]["max"] == undefined) {
		$('#myModalNewValue').attr("max", "");
	} else {
		$('#myModalNewValue').attr("max", lastEEPROMdump[key]["max"]);
	}
	if (lastEEPROMdump[key]["valid_values"] == undefined) {
		$('#myModalNotes').html( "" );
	} else {
		var validValues = lastEEPROMdump[key]["valid_values"] + ""; // Array to string
		$('#myModalNotes').html( "<u>Valid values</u>: "+validValues );
	}
	if (lastEEPROMdump[key]["default"] == undefined) {
		$("#myModalDefault").attr('txt', "");
		$("#myModalDefault").html("");
	} else {
		$("#myModalDefault").attr('txt', lastEEPROMdump[key]["default"]);
		$("#myModalDefault").html("Default value");
	}
	if (lastEEPROMdump[key]["disabled"] == undefined) {
		$("#myModalDisable").attr('txt', "");
		$("#myModalDisable").html("");
	} else {
		$("#myModalDisable").attr('txt', lastEEPROMdump[key]["disabled"]);
		$("#myModalDisable").html("Disable");
	}
	
	$('#myModal').modal('show');
	$('#myModal').on('shown.bs.modal', function () {
		$('#myModalNewValue').trigger('focus')
	});
}

function writeEEPROM() {
	var key = $('#myModalKey').val();
	var value = $('#myModalNewValue').val();
	if (lastEEPROMdump[key] == undefined) {
		console.log("Unknown key "+key);
		alert("Unknown key "+key);
	} else {
		if (lastEEPROMdump[key]["valid_values"] != undefined) {
			// check value and return if wrong!
			if (lastEEPROMdump[key]["valid_values"].indexOf(parseInt(value)) == -1) {
				$('#myModalNewValue').addClass("is-invalid");
				return;
			}
		}
		$.ajax({url: "data.php", data: {"writeEEPROM":"{\""+key+"\":{\"value\":"+value+"}}"}, success: function(result){ // , async: false
			if (result["error"] != undefined) {
				console.error("Error writeEEPROM(), error number: "+result["error"]["number"]+", error msg: "+result["error"]["msg"]);
				alert("Error writeEEPROM(), AJAX result see JS-Console!");
			} else {
				console.log(result["msg"].replace("###","\n"));
				$('#myModal').modal('hide');
				if ((key == "amount_ground_1") || // progress bar
					(key == "amount_ground_2") ||
					(key == "amount_coffee_preparations_until_next_cleaning") ||
					(key == "amount_rinsings_until_next_cleaning") ||
					(key == "amount_water") ||
					(key == "amount_calcified")) {
					
					var percentage = Math.round( (value-lastEEPROMdump[key]["min"]) / (lastEEPROMdump[key]["max"]-lastEEPROMdump[key]["min"]) * 100);
					$("#"+key+"-bar").attr("style", "width: "+percentage+"%;  cursor: pointer;");
					$("#"+key+"-bar").attr("aria-valuenow", value);
					$("#"+key+"-bar").text(percentage+"%");
					if (value > lastEEPROMdump[key]["max"]) {
						$("#"+key+"-bar").addClass("bg-danger");
					} else {
						$("#"+key+"-bar").removeClass("bg-danger");
					}
				} else { // or normal values
					$('#'+key).text(value);
				}
				if (key == "temperature") {
					$('#temperatureA').text(value);
					$('#temperatureB').text(value);
				}
			}
		}}).fail(function() { alert( "Error writing EEPROM data." ); });
	}
}

function configureIndividualCoffee(coffeeType) {
	$('#myModal2CoffeeType').val( coffeeType );
	$('#myModal2Title').html( "Configure your "+$('#'+coffeeType).text() );
	$('#myModal2NewValue').val( Cookies.get(coffeeType) );

	$('#myModal2').modal('show');
}
function defaultCookieText() {
	coffeeType = $('#myModal2CoffeeType').val();
	if (coffeeType == "small_coffee") {
		$('#myModal2NewValue').val('{"powder_quantity_small_coffee":{"value":5},"water_quantity_small_coffee":{"value":180},"temperature":{"value":0}}');
	} else if (coffeeType == "big_coffee") {
		$('#myModal2NewValue').val('{"powder_quantity_big_coffee":{"value":8},"water_quantity_big_coffee":{"value":340},"temperature":{"value":0}}');
	} else if (coffeeType == "special_coffee") {
		$('#myModal2NewValue').val('{"powder_quantity_special_coffee":{"value":11},"water_quantity_special_coffee":{"value":380},"temperature":{"value":0}}');
	}
}
function removeCookie() {
	coffeeType = $('#myModal2CoffeeType').val();
	Cookies.remove(coffeeType);

	$('#myModal2').modal('hide');
}
function setCookie() {
	coffeeType = $('#myModal2CoffeeType').val();
	Cookies.set(coffeeType, $('#myModal2NewValue').val(), {expires: 365*10});
	$('#myModal2').modal('hide');
}
function writeCookieToEEPROM() {
	coffeeType = $('#myModal2CoffeeType').val();
	var txt = Cookies.get(coffeeType);
	var obj = JSON.parse( txt );

	$.ajax({url: "data.php", data: { "writeEEPROM":txt }, success: function(result){ // , async: false
		if (result["error"] != undefined) {
			console.error("Error writeCookieToEEPROM(), error number: "+result["error"]["number"]+", error msg: "+result["error"]["msg"]);
			alert("Error writeEEPROM(), AJAX result see JS-Console!");
		} else {
			$.each(result["msg"].split("###"), function (i, val) {
				var ret = val.split(": ");
				if (ret[0] == "ok") {
					var key = ret[1];
					var value = obj[key]["value"];
					$('#'+key).text(value);
					if (key == "temperature") {
						$('#temperatureA').text(value);
						$('#temperatureB').text(value);
					}
					console.log(val);
				} else {
					if (val != "") {
						console.log("Unknown return value in writeCookieToEEPROM(): "+val);
					}
				}
			});

			$('#myModal2').modal('hide');
		}
	}}).fail(function() { alert( "Error writing EEPROM data from cookie." ); });
}

$(function() {
	$('#commandButton')
	.mouseenter(function() {
		if (!$('#commandButton').hasClass("disabled")) {
			$('#commandButton').hide();
			$('#commandBox').css("display","inline-block");
			$('#command').focus();
		}
	});
	$('#commandPreFill')
	.change(function() {
		$('#command').val( $(this).val() );
		$('#command').focus();
	});
	$('#commandBox')
	.mouseleave(function() {
		$('#commandButton').show();
		$('#commandBox').hide();
	});

	$('#commandForm').submit(function( event ) {
		command();
		event.preventDefault();
	});
});

function command() {
	if (!offline) {
		var cmd = $('#command').val();
		if ((cmd.length >= 3) && (cmd.includes(":"))) {
			$('#command').removeClass("is-invalid");

			$.ajax({url: "data.php", data: {"command":cmd}, success: function(result){ // , async: false
				if (result["error"] != undefined) {
					console.error("Error command(), error number: "+result["error"]["number"]+", error msg: "+result["error"]["msg"]);
					alert("Error command(), AJAX result see JS-Console!");
				} else {
					$('#command').val("");
					$('#commandResponse').text("Response: "+result["msg"]).show().fadeOut( 3000 ); // show response 3s
				}
			}}).fail(function() { alert( "Error sending your command to the machine." ); });
		} else {
			$('#command').addClass("is-invalid");
		}
	}
}

