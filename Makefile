CC      = g++
CFLAGS  = -Wall -Wextra -I/usr/include/libserial/
LDFLAGS = -lserial -lpthread -ljsoncpp -lstdc++fs

OBJS = EEPROM.o EEPROM_Status.o JsonFile.o RAM.o RAM_Status.o SerialConnection.o Storage.o color-definitions.h

.PHONY: clean dist-clean

all: JuraCoffeeMemory tools/display tools/display-screen-saver tools/formate-dump

clean:
	rm -f *.o

dist-clean: clean
	rm -f JuraCoffee formate-dump tools/display tools/display-screen-saver tools/formate-dump

%.o: %.cpp color-definitions.h
	$(CC) $(CFLAGS) -c $< -o $@

tools/display: SerialConnection.o tools/display.cpp
	$(CC) $(CFLAGS) SerialConnection.o tools/display.cpp -o tools/display $(LDFLAGS)

tools/display-screen-saver: SerialConnection.o tools/display-screen-saver.cpp
	$(CC) $(CFLAGS) SerialConnection.o tools/display-screen-saver.cpp -o tools/display-screen-saver $(LDFLAGS)

tools/formate-dump: tools/formate-dump.cpp
	$(CC) $(CFLAGS) tools/formate-dump.cpp -o tools/formate-dump

JuraCoffeeMemory: $(OBJS) JuraCoffeeMemory.cpp
	$(CC) $(CFLAGS) $(OBJS) JuraCoffeeMemory.cpp -o JuraCoffeeMemory $(LDFLAGS)

