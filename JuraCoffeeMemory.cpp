#include <iostream>
#include <string>

#include "EEPROM.hpp"
#include "SerialConnection.hpp"

using namespace std;

#define OFFLINE false

int main(int argc, char* argv[]) {
    string input;

    // TODO:
    // - device path
    // - log file path
    // (command call with default values?!)





    SerialConnection& s = SerialConnection::getInstance();
//    s.setPort("/dev/ttyACM1");
    s.connect();

    EEPROM* oldEEPROM = new EEPROM(OFFLINE);

    while (true) {
        //oldEEPROM->printRawVector();
        //oldEEPROM->printByteVector();

        //cout << "Anzahl an EEPROM: " << oldEEPROM->raw.size() << endl;
        //cout << "Anzahl an Bytes: " << oldEEPROM->bytes.size() << endl;

        cout << endl << "Enter a command to the coffee maschine, press only <Enter> to continue or <Q> to quit: ";
        getline(cin, input);
        if (input == "Q" || input == "q" || input == "quit" || input == "exit") {
            cout << "Bye." << endl;
            delete oldEEPROM;
            break;
        }
        if (input.length() > 1) {
            s.send(input);
            string resp = s.receive();
            cout << resp << endl;
        }

        EEPROM* newEEPROM = new EEPROM(OFFLINE);
        newEEPROM->diffBytesWith(oldEEPROM);

        delete oldEEPROM;
        oldEEPROM = newEEPROM;
    }
    s.disconnect();

    return 0;
}
