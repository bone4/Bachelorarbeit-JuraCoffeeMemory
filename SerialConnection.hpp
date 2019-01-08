#ifndef SERIALCONNECTION_H
#define SERIALCONNECTION_H

#include <stdio.h>
#include <iostream>
#include <string>

#include <signal.h>
#include <unistd.h> // alarm()

// lock file
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/file.h>

#include <SerialStream.h> // to communicate with a serial device
#include <algorithm>    /* std::remove() */
#include "color-definitions.h"

using namespace std;

class SerialConnection
{
    public:
        // Singleton Design Pattern
        static SerialConnection& getInstance();

        virtual ~SerialConnection();

        // Device name
        void setPort(string port);
        string getPort();
        // Command part of all commands
        void setCommandSuffix(string suffix);
        string getCommandSuffix();

        // Open and configure the hardware serial stream.
        void connect();
        // Close the hardware serial stream.
        void disconnect();

        bool testConnection();
        string testCommand;
        string testAnswer;
        void setTestCommand(string command);
        string getTestCommand();
        void setTestAnswer(string answer);
        string getTestAnswer();

        // Send data
        void send(string txt);
        // Receive data
        string receive();

    protected:

    private:
        // Singleton Design Pattern
        SerialConnection();
        static SerialConnection instance;
        // Copy constructor forbidden
        SerialConnection(const SerialConnection& other);
        // Assignment operator forbidden
        SerialConnection operator=(SerialConnection& other);

        // Reading Buffer array
        const int BUFFER_SIZE = 255; // see connect() params
//        char* input_buffer;

        string serialPort;
        string commandSuffix;

        // Instantiate a Serial Port and a Serial Stream object.
        LibSerial::SerialStream serial_stream;

        string removeSpecialCharFromString(string& txt);
};

#endif // SERIALCONNECTION_H
