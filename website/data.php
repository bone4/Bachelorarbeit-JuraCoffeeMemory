<?php
/*
 * Copyright (c) 2019 Niklas Krüger <niklas.krueger@tuhh.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
