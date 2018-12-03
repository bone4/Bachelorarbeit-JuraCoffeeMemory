#include "EEPROM.hpp"

using namespace std;

int EEPROM::run = 0;

EEPROM::EEPROM(bool offline, string filePath)
{
    //ctor
    setParams(EEPROM_command, EEPROM_row_length, EEPROM_rows, filePath); // read 1 row EEPROM wich consits of 64 hex chars

    work_offline = offline;

    if (!work_offline) { // ONLINE
        readStorage();
    } else { // OFFLINE
        unsigned int microseconds = 500000; // = 0.5s
        int i=-1; int ende = 16;

        run+=1;
        cout << endl << "Run: " << run << endl;

        /* 2 hex Char = 1 Byte; 32 Bytes pro Reihe; 16 Reihen; => 32*16=512 Byte EEPROM */
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        if (run==3) raw.push_back("0301000100060001000100000004000F00000000000000000001000F0000000F"); //  1
        else raw.push_back("0001000100060001000100000004000F00000000000000000001000F0000000E");
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        raw.push_back("000F000F00000000003F000E0000000000000000000000000001000000000000"); //  2
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        raw.push_back("0000000000000000030180000A0A00710038000000000C140000069700000000"); //  3
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        raw.push_back("0000000100000000000003C00000001E000000DC080480A00414012C00640276"); //  4
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        raw.push_back("000F00E6010A01BE0222073A08980BEA17D403E8044C05DC0BEA070807D00BB8"); //  5
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        raw.push_back("645D285F050502BC080D1000000300B43C00371E015050320190057802580000"); //  6
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        raw.push_back("0000000000000000000000000000000000000000000044781E0F08050A004B00"); //  7
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        raw.push_back("017C00060200000000000000050A0672064001900200017C001D001300010000"); //  8
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        raw.push_back("02410040050508080B0B000000B40154017C00B40154017C000C0672083406D6"); //  9
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        raw.push_back("101A121100BE06060A060640076C06720226028A0384089809C40BB8177002BC"); // 10
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        raw.push_back("3C0A0404040414063C040000010005053214140AF01400000000000000000000"); // 11
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        raw.push_back("0310000032240000070803E801F4012C000020050014141401C201C203E80000"); // 12
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        raw.push_back("1234000000000000000000000000000000000000000000000000000000000000"); // 13
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        raw.push_back("F00E7800463C040B0000001E00C8012C01900190000000000000000000000000"); // 14
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        raw.push_back("0000000000000000000000000000000000000000000000000000000000000000"); // 15
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);
        if (run==3) raw.push_back("02000000000000000000000000000000000000000000000000000000000001FF"); // 16
        else raw.push_back("0000000000000000000000000000000000000000000000000000000000000000"); // 16
		i++; progressBar(i, ende, "Progress:"); usleep(microseconds);

        microseconds = 1000000; // = 1s
        usleep(microseconds);
    } // OFFLINE



    for (auto &row : raw) {
        hexString2int(row, bytes);
    }
}

EEPROM::~EEPROM()
{
    //dtor
}
