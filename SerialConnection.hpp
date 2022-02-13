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

        int fd_lock_device_file;

        string serialPort;
        string commandSuffix;

        // Instantiate a Serial Port and a Serial Stream object.
        LibSerial::SerialStream serial_stream;

        string removeSpecialCharFromString(string& txt);
};

#endif // SERIALCONNECTION_H
