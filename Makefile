CC      = g++
CFLAGS  = -Wall -Wextra
LDFLAGS = -lserial -lpthread -ljsoncpp -lstdc++fs

OBJS = EEPROM.o JsonFile.o RAM.o SerialConnection.o Storage.o

.PHONY: clean dist-clean

all: JuraCoffeeMemory

clean:
	rm -f *.o

dist-clean: clean
	rm -f JuraCoffee

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

JuraCoffeeMemory: $(OBJS) JuraCoffeeMemory.cpp
	$(CC) $(CFLAGS) $(OBJS) JuraCoffeeMemory.cpp -o JuraCoffeeMemory $(LDFLAGS)

