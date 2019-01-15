<?php

header('Content-Type: application/json');
ob_start();

if (isset($_GET['getEEPROM'])) {
	passthru('../JuraCoffeeMemory eeprom 2>&1', $err);
} elseif (isset($_GET['getRAM'])) {
	passthru('../JuraCoffeeMemory ram 2>&1', $err);
} elseif (isset($_GET['writeEEPROM'])) {
	$cmd = 'echo '.escapeshellarg(urldecode($_GET['writeEEPROM'])).' | ../JuraCoffeeMemory eepromWrite 2>&1';
	echo '{"msg":"';
	passthru($cmd, $err);
	echo '"}';
} elseif (isset($_GET['command'])) {
	$cmd = 'echo '.escapeshellarg(urldecode($_GET['command'])).' | ../JuraCoffeeMemory command 2>&1';
	echo '{"msg":"';
	passthru($cmd, $err);
	echo '"}';
}

$output = ob_get_clean();

# remove special chars
$output = str_replace(array("\r","\n"), "", $output);

if (!empty($err)) {
	$data["error"]["msg"] = $output;
	$data["error"]["number"] = $err;
	echo json_encode($data);
} else {
	echo $output;
}

?>
