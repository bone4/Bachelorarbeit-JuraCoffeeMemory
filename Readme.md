# JuraCoffee
This C++ project is designed to read read and analyse the memory of a Jura Impressa S9 coffee maker.

## Requirements
libserial and libjsoncpp:
```sh
sudo apt-get install libserial-dev libboost-all-dev libjsoncpp-dev
```

with these linker flags to compile:
```
-lserial -lpthread -ljsoncpp
```

## Usage
Run:
```sh
make
./JuraCoffee
```

If you have problems to enable a serial connection, run the programm twice or try this:
```sh
screen /dev/ttyACM0 9600
exit
```
You can quit with <Ctrl>+D or kill the screen with <Ctrl>+A, k, y.

Unfortunately this not the same as
```sh
stty -F /dev/ttyACM0 9600 raw \
ignbrk -brkint -icrnl -imaxbel \
-opost -onlcr \
-isig -icanon -iexten -echo -echoe -echok -echoctl -echoke
```
it's sometimes a bit magical. But with the used libserial library the data are reliably send and received.

## Remarks
The Arduino file is used from [E-17 CoffeeMachine](https://collaborating.tuhh.de/e-17/General/CoffeeMachine/tree/master/arduino).

More usefull links:
[libserial Github](https://github.com/crayzeewulf/libserial), [libserial documentation](https://libserial.readthedocs.io/en/latest/index.html)
[libjsoncpp](https://en.wikibooks.org/wiki/JsonCpp)

### Command endings (important if you need the expacted length)
| Arduino -> Jura CoffeeMachine | Jura CoffeeMachine -> Arduino |
| ----------------------------- | ----------------------------- |
| ```TY:\r\n```                 | ```ty:E1300 CAPU 3\r```       |

| Char         | hex | dez |
| ------------ | --- | --- |
| CR: ```\r``` | 0D  | 13  |
| LF: ```\n``` | 0A  | 10  |



