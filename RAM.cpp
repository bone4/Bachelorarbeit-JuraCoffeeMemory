#include "RAM.hpp"

using namespace std;

int RAM::run = 0;

RAM::RAM(bool offline, string filePath) :
    Storage("RT:", RAM_row_length), // read 1 row RAM wich consits of 32 hex chars
    logFilePath(filePath)
{
    //ctor
    work_offline = offline;

    if (!work_offline) { // ONLINE

    } else { // OFFLINE

    }

}

RAM::~RAM()
{
    //dtor
}

void RAM::diffBytesWith(RAM* last) {

}
