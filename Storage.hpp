#ifndef STORAGE_H
#define STORAGE_H

#include <vector>

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <iomanip>
#include <math.h>       /* floor */
#include <algorithm>    /* std::remove() */
#include "SerialConnection.hpp"

class Storage
{
    public:
        Storage(std::string command, int length);
        virtual ~Storage();

        void printRawVector();
        string getRawData();
        void printVector(std::vector<int> const &v);
        void printByteVector();
        void printByteVectorShort();

        std::vector<string> raw;
        std::vector<int> bytes;
        bool work_offline;

        void readStorage(int Position);

    protected:
        // This function assumes src to be a zero terminated sanitized string with
        // an even number of [0-9a-f] characters
        void hexString2int(std::string src, std::vector<int> &target);
        int char2int(char input);

    private:
        std::string command;
        int length; // EEPROM = 3 (rt:) + 2*32 Bytes; 1 word = 2 bytes

        string removeSpecialCharFromString(string& txt);
};

#endif // STORAGE_H
