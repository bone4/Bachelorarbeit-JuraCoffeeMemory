#include <iostream>
#include <cstdlib> // GetInput()
#include <string>

#include "EEPROM.hpp"
#include "RAM.hpp"
#include "SerialConnection.hpp"

using namespace std;

#define OFFLINE false

void eeprom(string& eepromPath) {
    SerialConnection& s = SerialConnection::getInstance();
    s.connect();

    string input;

    EEPROM* oldEEPROM = new EEPROM(OFFLINE, eepromPath);

    while (true) {
        //oldEEPROM->printRawVector();
        //oldEEPROM->printByteVector();

        //cout << "Anzahl an EEPROM: " << oldEEPROM->raw.size() << endl;
        //cout << "Anzahl an Bytes: " << oldEEPROM->bytes.size() << endl;

        cout << endl << "EEPROM: Enter a command to the coffee maschine, press only <Enter> to continue or <Q> to quit: ";
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

        EEPROM* newEEPROM = new EEPROM(OFFLINE, eepromPath);
        newEEPROM->diffBytesWith(oldEEPROM);

        delete oldEEPROM;
        oldEEPROM = newEEPROM;
    }

    s.disconnect();
}

void ram(string& ramPath) {
    SerialConnection& s = SerialConnection::getInstance();
    s.connect();

    string input;

    RAM* oldRAM = new RAM(OFFLINE, ramPath);

    while (true) {
        //oldRAM->printRawVector();
        //oldRAM->printByteVector();

        //cout << "Anzahl an RAM: " << oldRAM->raw.size() << endl;
        //cout << "Anzahl an Bytes: " << oldRAM->bytes.size() << endl;

        cout << endl << "RAM: Enter a command to the coffee maschine, press only <Enter> to continue or <Q> to quit: ";
        getline(cin, input);
        if (input == "Q" || input == "q" || input == "quit" || input == "exit") {
            cout << "Bye." << endl;
            delete oldRAM;
            break;
        }
        if (input.length() > 1) {
            s.send(input);
            string resp = s.receive();
            cout << resp << endl;
        }

        RAM* newRAM = new RAM(OFFLINE, ramPath);
        newRAM->diffBytesWith(oldRAM);

        delete oldRAM;
        oldRAM = newRAM;
    }

    s.disconnect();
}

void DisplayMainMenu() {
    cout << "Main Menu\n";
    cout << "Please make your selection\n";
    cout << "1 - EEPROM\n";
    cout << "2 - Ram\n";
    cout << "3 - Options\n";
    cout << "4 - Quit\n";
    cout << "Selection: ";
}
void DisplayOptionsMenu(string& devicePath, string& eepromPath, string& ramPath) {
    cout << "Options Menu\n";
    cout << "1 - Device path (" << devicePath << ")\n";
    cout << "2 - EEPROM log file path (" << eepromPath << ")\n";
    cout << "3 - Ram log file path (" << ramPath << ")\n";
    cout << "4 - Back\n";
    cout << "Selection: ";
}
void OptionsMenu(string& devicePath, string& eepromPath, string& ramPath) {
    string choice = "";
    do {
        system("clear");
        DisplayOptionsMenu(devicePath, eepromPath, ramPath);
        getline(cin, choice);
        if (choice == "1") {
            cout << "Device path: ";
            getline(cin, devicePath);
            SerialConnection& s = SerialConnection::getInstance();
            s.setPort(devicePath);
        } else if (choice == "2") {
            cout << "EEPROM log file path: ";
            getline(cin, eepromPath);
        } else if (choice == "3") {
            cout << "Ram log file path: ";
            getline(cin, ramPath);
        }
    } while(choice!="4");
}
void MainMenu(string& devicePath, string& eepromPath, string& ramPath) {
    string choice = "";
    do {
        system("clear");
        DisplayMainMenu();
        getline(cin, choice);
        if (choice == "1") { // EEPROM
                eeprom(eepromPath);
        } else if (choice == "2") { // RAM
                ram(ramPath);
        } else if (choice == "3") {
                OptionsMenu(devicePath, eepromPath, ramPath);
        }
    } while(choice!="4");
}

// int main(int argc, char* argv[]) {
int main() {
    string devicePath = "/dev/ttyACM0";
    string eepromPath = "data/eeprom.json";
    string ramPath = "data/ram.json";

    MainMenu(devicePath, eepromPath, ramPath);

    // system("PAUSE");
    return 0;
}
