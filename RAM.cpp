#include "RAM.hpp"

using namespace std;

int RAM::run = 0;

RAM::RAM(bool offline, string filePath)
{
    //ctor
    setParams(RAM_command, RAM_row_length, RAM_rows, filePath); // read 1 row RAM wich consits of 32 hex chars

    work_offline = offline;

    if (!work_offline) { // ONLINE
        readStorage();
    } else { // OFFLINE

    }



    for (auto &row : raw) {
        hexString2int(row, bytes);
    }
}

RAM::~RAM()
{
    //dtor
}
