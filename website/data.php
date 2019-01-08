<?php

header('Content-Type: application/json');
ob_start();

if (isset($_GET['getEEPROM'])) {
	passthru('../JuraCoffeeMemory eeprom 2>&1', $err);
} elseif (isset($_GET['getRAM'])) {
	passthru('../JuraCoffeeMemory ram 2>&1', $err);
} elseif (isset($_GET['writeEEPROM']) && isset($_GET['key']) && isset($_GET['value'])) {
	$cmd = 'echo \'{"'.escapeshellarg($_GET['key']).'":{"value":'.escapeshellarg($_GET['value']).'}}\' | ../JuraCoffeeMemory eepromWrite 2>&1';
	#echo $cmd."\n";
	echo '{"msg":"';
	passthru($cmd, $err);
	echo '"}';
} elseif (isset($_GET['command'])) {
	// TODO
}

$output = ob_get_clean();

if (!empty($err)) {
	$data["error"]["msg"] = $output;
	$data["error"]["number"] = $err;
	echo json_encode($data);
} else {
	echo $output;
}

# Notes for C++ to ensure only 1 serial connection for multiple process instances:
# https://www.google.com/search?q=c%2B%2B+lock+file
# https://www.linuxquestions.org/questions/programming-9/locking-files-with-c-751872/

?>
