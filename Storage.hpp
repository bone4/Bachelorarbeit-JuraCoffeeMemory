#ifndef STORAGE_H
#define STORAGE_H

#include <vector>

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>     /* usleep() */
#include <iomanip>
#include <math.h>       /* floor */
#include <algorithm>    /* std::remove(), std::find() */
#include <sys/ioctl.h>  /* Terminal settings: ioctl() */
#include "SerialConnection.hpp"
#include "JsonFile.hpp"
#include "color-definitions.h"

class Storage
{
    public:
        Storage();
        void setParams(std::string command, int row_length, int rows, std::string filePath);
        virtual ~Storage();

        void printRawVector();
        string getRawData();
        void printVector(std::vector<int> const &v);
        void printByteVector();
        void printByteVectorShort();

        std::vector<string> raw;
        std::vector<int> bytes;
        bool work_offline;

        void readStorage();
        void readStorage(int Position);
        // This function assumes src to be a zero terminated sanitized string with
        // an even number of [0-9a-f] characters
        void hexString2int(std::string src, std::vector<int> &target);
        int char2int(char input);

        void diffBytesWith(Storage* last, vector<int> excludeBytes = vector<int>(), bool writeIntoJsonFile = true, std::string comment="");

    protected:
        // This function prints the progress bar for the real reading and the offline reading (EEPROM or RAM)
        void progressBar(int num, int end, std::string txt);

        std::string removeSpecialCharFromString(std::string& txt);

        bool getBit(unsigned char byte, int position); // position in range 0-7

    private:
        std::string command;
        int storage_row_length;
        int storage_rows;
        int length; // EEPROM = 3 (rt:) + 2*32 Bytes; 1 word = 2 bytes
        std::string logFilePath;
};

#endif // STORAGE_H
