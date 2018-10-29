#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <iomanip>
#include <vector>

using namespace std;

// #define OFFLINE yes!

void printVector(vector<int> const &v) {
    for (auto &it : v) {
        printf("%d\t%02X\t\n", it, it);
//        cout << dec << it << "\t";
//        cout << hex << it << dec << "\t" << "size: " << v.size() << endl;
    }
    return;
}

int char2int(char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  throw std::invalid_argument("Invalid input string");
}

// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters
void hexString2int(const char* src, vector<int> &target) {
  while(*src && src[1])
  {
    target.push_back(char2int(*src)*16 + char2int(src[1]));
    src += 2;
  }
}

int main(int argc, char* argv[]) {
    vector<char*> eeprom;
    vector<int> bytes;

#ifndef OFFLINE
    string str;
    fstream f;

    int length = 67; // = 3 (rt:) + 2*32 Bytes; 1 word = 2 bytes // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Main function call?
    string device = "/dev/ttyACM0"; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Main function call!
    string command = "RT:"; // read on line of eeprom // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Main function call!

    for (int i = 0; i < 256; i+=16) {
        f.open(device, fstream::in | fstream::out); // O_RDWR | O_NOCTTY | O_NONBLOCK

        cout << "Eingabe:\t";
        cout << command;
        cout << setfill('0') << setw(2) << hex << uppercase << i;
        cout << "\r\n";

        f << "RT:";
        f << setfill('0') << setw(2) << hex << uppercase << i;
        f << "\r\n";
        //usleep(3000000);

        char* buffer = new char[length];
        f.read (buffer,length);
        eeprom.push_back(buffer+3); // +3 to cut off "rt:" // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Main function call?
        cout << "Ausgabe:\t" << string(buffer) << endl;

        cout << endl;

        f.close();
    }
#endif // OFFLINE
#ifdef OFFLINE
    /* 2 hex Char = 1 Byte; 32 Bytes pro Reihe; 16 Reihen; => 32*16=512 Byte EEPROM */
    eeprom.push_back("0001000100060001000100000004000F00000000000000000001000F0000000E");
    eeprom.push_back("000F000F00000000003F000E0000000000000000000000000001000000000000");
    eeprom.push_back("0000000000000000030180000A0A00710038000000000C140000069700000000");
    eeprom.push_back("0000000100000000000003C00000001E000000DC080480A00414012C00640276");
    eeprom.push_back("000F00E6010A01BE0222073A08980BEA17D403E8044C05DC0BEA070807D00BB8");
    eeprom.push_back("645D285F050502BC080D1000000300B43C00371E015050320190057802580000");
    eeprom.push_back("0000000000000000000000000000000000000000000044781E0F08050A004B00");
    eeprom.push_back("017C00060200000000000000050A0672064001900200017C001D001300010000");
    eeprom.push_back("02410040050508080B0B000000B40154017C00B40154017C000C0672083406D6");
    eeprom.push_back("101A121100BE06060A060640076C06720226028A0384089809C40BB8177002BC");
    eeprom.push_back("3C0A0404040414063C040000010005053214140AF01400000000000000000000");
    eeprom.push_back("0310000032240000070803E801F4012C000020050014141401C201C203E80000");
    eeprom.push_back("1234000000000000000000000000000000000000000000000000000000000000");
    eeprom.push_back("F00E7800463C040B0000001E00C8012C01900190000000000000000000000000");
    eeprom.push_back("0000000000000000000000000000000000000000000000000000000000000000");
    eeprom.push_back("0000000000000000000000000000000000000000000000000000000000000000");
#endif // OFFLINE
    for (auto &row : eeprom) {
        cout << string(row) << endl;
        hexString2int(row, bytes);
    }

    printVector(bytes);

    cout << "Anzahl an EEPROM: " << eeprom.size() << endl;
    cout << "Anzahl an Bytes: " << bytes.size() << endl;

    return 0;
}



