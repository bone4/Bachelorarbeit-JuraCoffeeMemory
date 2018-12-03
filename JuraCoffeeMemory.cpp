#include <iostream>
#include <cstdlib> // GetInput()
#include <string>
#include <vector>
#include <algorithm> // sort
#include <unistd.h> // usleep()

#include "Storage.hpp"
#include "EEPROM.hpp"
#include "RAM.hpp"
#include "SerialConnection.hpp"
#include "JsonFile.hpp"
#include "color-definitions.h"

using namespace std;

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#include <sstream>

#define OFFLINE false
// #define OFFLINE true

void eeprom(string& eepromPath) {
    SerialConnection& s = SerialConnection::getInstance();
    if (!OFFLINE) s.connect();

    string input;

    EEPROM* oldEEPROM = new EEPROM(OFFLINE, eepromPath);

    while (true) {
        cout << endl << COLOR_FG_green << "EEPROM: Enter a command to the coffee maschine, press only <Enter> to continue, <S> to print the last dump or <Q> to quit:" << COLOR_reset << " ";
        getline(cin, input);
        if (input == "Q" || input == "q" || input == "quit" || input == "exit") {
            cout << "Bye." << endl;
            delete oldEEPROM;
            break;
        }
        if (input == "S" || input == "s") {
            oldEEPROM->printRawVector();
            //oldEEPROM->printByteVector();
            //cout << "Anzahl an EEPROM: " << oldEEPROM->raw.size() << endl;
            //cout << "Anzahl an Bytes: " << oldEEPROM->bytes.size() << endl;
        } else {
            if (input.length() > 1) {
                if (!OFFLINE) s.send(input);
                string resp;
                if (!OFFLINE) resp = s.receive();
                if (!OFFLINE) cout << resp << endl;

                unsigned int microseconds = 2000000; // corresponds to 2s
                usleep(microseconds);
            }

            EEPROM* newEEPROM = new EEPROM(OFFLINE, eepromPath);
            newEEPROM->diffBytesWith(oldEEPROM);

            delete oldEEPROM;
            oldEEPROM = newEEPROM;
        }
    }

    s.disconnect();
}

void ram(string& ramPath) {
    SerialConnection& s = SerialConnection::getInstance();
    if (!OFFLINE) s.connect();

    string input;

    RAM* oldRAM = new RAM(OFFLINE, ramPath);

    while (true) {
        cout << endl << COLOR_FG_green << "RAM: Enter a command to the coffee maschine, press only <Enter> to continue, <S> to print the last dump or <Q> to quit:" << COLOR_reset << " ";
        getline(cin, input);
        if (input == "Q" || input == "q" || input == "quit" || input == "exit") {
            cout << "Bye." << endl;
            delete oldRAM;
            break;
        }
        if (input == "S" || input == "s") {
            oldRAM->printRawVector();
            //oldRAM->printByteVector();
            //cout << "Anzahl an RAM: " << oldRAM->raw.size() << endl;
            //cout << "Anzahl an Bytes: " << oldRAM->bytes.size() << endl;
        } else {
            if (input.length() > 1) {
                if (!OFFLINE) s.send(input);
                string resp;
                if (!OFFLINE) resp = s.receive();
                if (!OFFLINE) cout << resp << endl;

                unsigned int microseconds = 2000000; // corresponds to 2s
                usleep(microseconds);
            }

            RAM* newRAM = new RAM(OFFLINE, ramPath);
            vector<int> excludeBytes {2, 9, 12, 28, 29, 30, 31, 32, 33, 39, 40, 42, 44, 46, 65, 102, 103, 106, 108, 112, 124, 148, 149, 151, 156, 157, 162, 241, 252, 253, 254, 255};
            newRAM->diffBytesWith(oldRAM, excludeBytes);

            delete oldRAM;
            oldRAM = newRAM;
        }
    }

    s.disconnect();
}

void DisplayMainMenu() {
    cout << COLOR_BG_blue << "Main Menu" << COLOR_reset << endl
         << COLOR_FG_blue
         << "Please make your selection" << endl
         << "1 - EEPROM Skript" << endl
         << "2 - Ram Skript" << endl
         << "3 - " << endl
         << "4 - Send a command" << endl
         << "5 - " << endl
         << "6 - Dump EEPROM" << endl
         << "7 - Dump RAM" << endl
         << "8 - " << endl
         << "9 - Options" << endl
         << "0 - Analyse existing dumps" << endl
         << "Q / q / quit / exit - To leave" << endl
         << COLOR_reset
         << COLOR_FG_green << "Selection:" << COLOR_reset << " ";
}
void DisplayOptionsMenu(string& devicePath, string& eepromPath, string& ramPath) {
    cout << COLOR_BG_blue << "Options Menu" << COLOR_reset << endl
         << COLOR_FG_blue
         << "1 - Device path (" << devicePath << ")" << endl
         << "2 - EEPROM log file path (" << eepromPath << ")" << endl
         << "3 - Ram log file path (" << ramPath << ")" << endl
         << "4 - Back" << endl
         << COLOR_reset
         << COLOR_FG_green "Enter the number or <Q> to quit:" << COLOR_reset << " ";
}
void AnalyseFileDumpsMenu(string filename) {
    // load JSON file
    JsonFile& jf = JsonFile::getInstance();
    jf.readJson(filename);

    string choice = "";
    while(true) {
        // Output
        system("clear");
        cout << COLOR_BG_blue << "Analyse Dumps Menu -> " << COLOR_BG_cyan << filename << COLOR_reset << endl
             << COLOR_FG_blue;
        cout << "Please select a concrete dump\n";
        // Format Json-"timestamp" data
        int n=0;
        vector<dump> dumps = jf.getDumps();
        for(auto it = dumps.begin(); it != dumps.end(); ++it) {
            cout << to_string(n) << " - " << it->timestamp << ": " << it->comment << endl;
            n++;
        }
        cout << COLOR_reset;
        while(true) {
            cout << COLOR_FG_green << "Enter the number, <C> to clear or <Q> to quit:" << COLOR_reset << " ";
            // Input from the user
            getline(cin, choice);
            if (choice == "Q" || choice == "q" || choice == "quit" || choice == "exit") {
                return;
            }
            if (choice == "C" || choice == "c" || choice == "clear") {
                break;
            }
            try {
                unsigned int choiceInt = stoi(choice);
                if (stoi(choice) < 0 || choiceInt > dumps.size()-1) {
                    continue;
                }
                vector<dump>::iterator DumpIterator = dumps.begin() + choiceInt;
                cout << "Your choice is: " << DumpIterator->timestamp << ": " << DumpIterator->comment << endl;

                //cout << DumpIterator->timestamp << "; " << DumpIterator->comment << "; " << DumpIterator->rawOld << "; " << DumpIterator->rawNew << "; " << endl;
                // Create instances with the RAW data
                Storage* sOld = new Storage;
                Storage* sNew = new Storage;
                sOld->setParams("", DumpIterator->rawOld.length()/16, 16, filename); // x,64,16,x = EEPROM
                sNew->setParams("", DumpIterator->rawNew.length()/16, 16, filename); // x,32,16,x = RAM
                sOld->raw.push_back(DumpIterator->rawOld);
                sNew->raw.push_back(DumpIterator->rawNew);
                sOld->hexString2int(DumpIterator->rawOld, sOld->bytes);
                sNew->hexString2int(DumpIterator->rawNew, sNew->bytes);
                // compare the Bytes
                vector<int> excludeBytes;
                // Excluded Bytes for the RAM:
                //vector<int> excludeBytes {2, 9, 12, 28, 29, 30, 31, 32, 33, 39, 40, 42, 44, 46, 65, 102, 103, 106, 108, 112, 124, 148, 149, 151, 156, 157, 162, 241, 252, 253, 254, 255};
                sNew->diffBytesWith(sOld, excludeBytes, false, DumpIterator->comment);
                // clean up
                delete sOld;
                delete sNew;
            } catch (...) {} // skip and ask again if no number was entered
        }
    }
    /*
    // Format Json-"data" data
    vector<data> dataAll = jf.getData();
    for(auto it = dataAll.begin(); it != dataAll.end(); ++it) {
        cout << it->byte << ": ";
        for(auto it2 = it->comments.begin(); it2 != it->comments.end(); ++it2) {
            cout << (*it2) << " ___ ";
        }
        cout << endl;
    }

    cout << "Press <Enter> to continue." << endl;
    string input;
    getline(cin, input);
    */
}
vector<string> AnalyseFilesListAll(string path) {
    vector<string> FileNames;

    // Select all JSON files
    for(fs::directory_iterator iter(path), end ;iter != end; ++iter) {
        if (iter->path().extension() == ".json") {
            stringstream ss;
            ss << path << iter->path().filename().string();
            FileNames.push_back(ss.str());
        }
    }
    sort(FileNames.begin(), FileNames.end());

    // Print the JSON files with an unique number
    int n=0;
	for( auto it = FileNames.begin(); it != FileNames.end(); ++it) {
		cout << to_string(n) << " - " << *it << endl;
		n++;
	}

    return FileNames;
}
void AnalyseFilesMenu(string& path) {
    string choice = "";
    while(true) {
        // Output
        system("clear");
        cout << COLOR_BG_blue << "Analyse Dumps Menu" << COLOR_reset << endl;
        cout << COLOR_FG_blue << "Please select a dump file\n";
        vector<string> FileNames = AnalyseFilesListAll(path);
        cout << COLOR_reset;
        cout << COLOR_FG_green << "Enter the number or <Q> to quit:" << COLOR_reset << " ";
        // Input from the user
        getline(cin, choice);
        if (choice == "Q" || choice == "q" || choice == "quit" || choice == "exit") {
            return;
        }
        try {
            unsigned int choiceInt = stoi(choice);
            if (stoi(choice) < 0 || choiceInt > FileNames.size()-1) {
                continue;
            }
            vector<string>::iterator FileNameIterator = FileNames.begin() + choiceInt;
            cout << "Your choice is: " << *FileNameIterator << endl;

            AnalyseFileDumpsMenu(*FileNameIterator);
        } catch (...) {} // skip and ask again if no number was entered
    }
}
void OptionsMenu(string& devicePath, string& eepromPath, string& ramPath) {
    string choice = "";
    do {
        system("clear");
        DisplayOptionsMenu(devicePath, eepromPath, ramPath);
        getline(cin, choice);
        if (choice == "1") {
            cout << COLOR_FG_magenta << "Device path:" << COLOR_reset << " ";
            getline(cin, devicePath);
            SerialConnection& s = SerialConnection::getInstance();
            s.setPort(devicePath);
        } else if (choice == "2") {
            cout << COLOR_FG_magenta << "EEPROM log file path:" << COLOR_reset << " ";
            getline(cin, eepromPath);
        } else if (choice == "3") {
            cout << COLOR_FG_magenta << "Ram log file path:" << COLOR_reset << " ";
            getline(cin, ramPath);
        }
    } while(choice!="4" && choice != "Q" && choice != "q" && choice != "quit" && choice != "exit");
}
void MainMenu(string& devicePath, string& path, string eepromPath, string ramPath) {
    string choice = "";
    string input; // input in a sub menu
    do {
        system("clear");
        DisplayMainMenu();
        getline(cin, choice);
        if (choice == "1") { // EEPROM
            eeprom(eepromPath);
        } else if (choice == "2") { // RAM
            ram(ramPath);
        } else if (choice == "4") { // Commands
            SerialConnection& s = SerialConnection::getInstance();
            if (!OFFLINE) s.connect();

            while (true) {
               cout << endl << COLOR_FG_green << "Enter a command to the coffee maschine or <Q> to quit:" << COLOR_reset << " ";
                getline(cin, input);
                if (input == "Q" || input == "q" || input == "quit" || input == "exit") {
                    break;
                }
                if (input.length() > 1) {
                    if (!OFFLINE) s.send(input);
                    string resp;
                    if (!OFFLINE) resp = s.receive();
                    if (!OFFLINE) cout << resp << endl;
                }
            }

            s.disconnect();
        } else if (choice == "6") { // Dump EEPROM
            SerialConnection& s = SerialConnection::getInstance();
            if (!OFFLINE) s.connect();

            while (true) {
                EEPROM* newEEPROM = new EEPROM(OFFLINE, eepromPath);
                newEEPROM->printRawVector();
                delete newEEPROM;

                cout << endl << COLOR_FG_green << "Hit <Enter> for a new EEPROM dump or press <Q> to quit:" << COLOR_reset << " ";
                getline(cin, input);
                if (input == "Q" || input == "q" || input == "quit" || input == "exit") {
                    break;
                }
            }

            s.disconnect();
        } else if (choice == "7") { // Dump RAM
            SerialConnection& s = SerialConnection::getInstance();
            if (!OFFLINE) s.connect();

            while (true) {
                RAM* newRAM = new RAM(OFFLINE, ramPath);
                newRAM->printRawVector();
                delete newRAM;

                cout << endl << COLOR_FG_green << "Hit <Enter> for a new RAM dump or press <Q> to quit:" << COLOR_reset << " ";
                getline(cin, input);
                if (input == "Q" || input == "q" || input == "quit" || input == "exit") {
                    break;
                }
            }

            s.disconnect();
        } else if (choice == "9") { // Options
            OptionsMenu(devicePath, eepromPath, ramPath);
        } else if (choice == "0") { // Analyse existing dumps
            AnalyseFilesMenu(path);
        }
    } while(choice!="Q" && choice!="q" && choice!="quit" && choice!="exit");
    cout << "Bye." << endl;
}

// int main(int argc, char* argv[]) {
int main() {
    string devicePath = "/dev/ttyACM0";

    string path = "data/";
    stringstream eepromPath;
    eepromPath << path << "eeprom.json";
    stringstream ramPath;
    ramPath << path << "ram.json";

    MainMenu(devicePath, path, eepromPath.str(), ramPath.str());

    // system("PAUSE");
    return 0;
}
