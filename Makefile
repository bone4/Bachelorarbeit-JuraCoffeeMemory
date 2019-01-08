CC      = g++
CFLAGS  = -Wall -Wextra
LDFLAGS = -lserial -lpthread -ljsoncpp -lstdc++fs

OBJS = EEPROM.o EEPROM_Status.o JsonFile.o RAM.o RAM_Status.o SerialConnection.o Storage.o color-definitions.h

.PHONY: clean dist-clean

all: JuraCoffeeMemory formate-dump

clean:
	rm -f *.o

dist-clean: clean
	rm -f JuraCoffee formate-dump

%.o: %.cpp color-definitions.h
	$(CC) $(CFLAGS) -c $< -o $@

JuraCoffeeMemory: $(OBJS) JuraCoffeeMemory.cpp
	$(CC) $(CFLAGS) $(OBJS) JuraCoffeeMemory.cpp -o JuraCoffeeMemory $(LDFLAGS)

formate-dump: formate-dump.cpp
	$(CC) $(CFLAGS) formate-dump.cpp -o formate-dump
