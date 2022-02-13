# JuraCoffee
This C++ project is designed to read and analyse the memory of a Jura Impressa S9 coffee maker.

## Requirements
libserial and libjsoncpp:
```sh
sudo apt-get install libserial-dev libboost-all-dev libjsoncpp-dev
```

with these linker flags to compile:
```
-lserial -lpthread -ljsoncpp
```

A webserver and PHP for the website with permission to the website directory:
```sh
sudo apt-get install php php-cli apache2
nano /etc/apache2/sites-enabled/000-default.conf
# Change: "DocumentRoot" to pathToThisProject/JuraCoffeeMemory/website/
# Add: <Directory pathToThisProject/JuraCoffeeMemory/website/>Require all granted</Directory>
service apache2 restart
```

Make sure that **your user** and **www-data** for the website are member of the group **dialout**. If not, you can add them by:
```sh
sudo usermod -aG dialout www-data
sudo usermod -aG dialout YourUsername
```

## Usage
Run:
```sh
make
./JuraCoffeeMemory
```

If you have problems to enable a serial connection, run the programm twice or try this:
```sh
screen /dev/ttyACM0 9600
exit
```
You can quit with \<Ctrl\>+D or kill the screen with \<Ctrl\>+A, k, y.

Unfortunately this not the same as
```sh
stty -F /dev/ttyACM0 9600 raw \
ignbrk -brkint -icrnl -imaxbel \
-opost -onlcr \
-isig -icanon -iexten -echo -echoe -echok -echoctl -echoke
```
it's sometimes a bit magically. But with the used libserial library the data are reliably send and received.

Call one of the following commands to get a JSON string to stdout, send a JSON string to update the EEPROM or send a command to the coffee machine:
```sh
./JuraCoffeeMemory ram
./JuraCoffeeMemory eeprom
./JuraCoffeeMemory eepromWrite
./JuraCoffeeMemory command
```
The last two commands expect input from stdin.

To test the website call:
```http
http://localhost/phpinfo.php
```
and finally:
```http
http://localhost/
```

### Return codes
| No. | Message | Recommended action |
| ---: | --- | --- |
|   0 | Ok, every thing is fine | nothing |
|   1 | JSON string could not be processed | Check your input or the mentioned file. |
|   2 | Alarm, no answer within 3 seconds. | Check your input using the commands file. Ignore this message if you tried a display test AN:03 |
|   3 | Could not open serial port | Check the arduino device path and enter the actual one in the option menu (-> 9 -> 1) |
|   4 | Could not configure serial port. | Run the arduino script inside the Arduino IDE and use the serial monitor to communicate with the coffee machine. |
|   5 | The test command failed. | Enter the result of the test command "TY:" in line 5 of SerialConnection.cpp, compile and run again. |
|   6 | Information of 256 bytes RAM are needed. | Debug the answer of the coffee machine in RAM_Status.cpp |
|   7 | Unknown byte in RAM_Status::getEntriesRAM() | Enter only byte numbers between 0-255. Have a look on the thesis results and enter only known and existing positions. The valiable known_bytes should have an entry at each position where the variable raw has queried non empty values. |
|   8 | Information of 512 bytes EEPROM are needed | Debug the answer of the coffee machine in EEPROM_Status.cpp |
|   9 | Unknown byte in RAM_Status::getEntriesRAM() | see 7. |
|  10 | JSON parsing error. | Check your JSON input on valid syntax. |
|  11 | The answer of the coffe machine didn't the expected one. | Debug the communication, fix a misspelled command or update the eypected answer in line 7 of EEPROM_Status.cpp |
|  12 | Wrong byte position in the second field of EEPROM_Status::getEntriesEEPROM() | A word consists of two bytes. Enter only 0, 1 or both. |
|  13 | The new EEPROM value for a whole word is out of range. | The JSON input value has to be between 0 and 65535 for a two bytes word. |
|  14 | The new EEPROM value for a single byte inside a word is out of range. | The JSON input value has to be between 0 and 255. |
|  15 | No valid input was made as command to the coffee machine. | Pass a valid input to stdin. |
| 225 | Device file couldn't be locked. | Check the connection to the arduino and terminate existing processes. |


## Remarks
The Arduino file is used from [E-17 CoffeeMachine](https://collaborating.tuhh.de/e-17/General/CoffeeMachine/tree/master/arduino).

More usefull links:
[libserial Github](https://github.com/crayzeewulf/libserial), [libserial documentation](https://libserial.readthedocs.io/en/latest/index.html);
[libjsoncpp](https://en.wikibooks.org/wiki/JsonCpp);
Website: [Twitter Bootstrap](https://getbootstrap.com/); [jQuery](https://jquery.com/); [JavaScript Cookie](https://github.com/js-cookie/js-cookie); [Intro.js](https://introjs.com/); 
[gitlab-markdown](https://collaborating.tuhh.de/help/user/markdown.md), [online-markdown](https://dillinger.io/)

### Command endings (important if you need the expacted length)
| Arduino -> Jura CoffeeMachine | Jura CoffeeMachine -> Arduino |
| ----------------------------- | ----------------------------- |
| ```TY:\r\n```                 | ```ty:E1300 CAPU 3\r```       |

| Char         | hex | dez |
| ------------ | --- | --- |
| CR: ```\r``` | 0D  | 13  |
| LF: ```\n``` | 0A  | 10  |

## DOI
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.6068038.svg)](https://doi.org/10.5281/zenodo.6068038)

