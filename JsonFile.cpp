#include "JsonFile.hpp"

JsonFile::JsonFile() :
    jsonFileStreamReadEEPROM("data/eeprom.json")
{
    //ctor
    bool b = reader.parse(jsonFileStreamReadEEPROM, eepromJson);     // Reader can also read strings
    if (!b) {
        cerr << "Error: " << reader.getFormattedErrorMessages();
        exit(9);
    }
}

JsonFile& JsonFile::getInstance() {
    // Singleton Design Pattern
    return instance;
}

// call constructor
JsonFile JsonFile::instance;

JsonFile::~JsonFile()
{
    //dtor
}

void JsonFile::eepromData(int i, string s) {
    stringstream ss, data;
    ss << setw(3) << setfill('0') << i;

    data << eepromJson["data"][ss.str()].asString() << s << "###";
    eepromJson["data"][ss.str()] = data.str();

    this->saveChanges();
}

void JsonFile::eepromLog(string rawOld, string rawNew, string comment) {
    auto t = time(nullptr);
    auto tm = *localtime(&t);
    stringstream ss;
    ss << put_time(&tm, "%Y-%m-%d %H-%M-%S");
    eepromJson[ss.str()]["rawOld"] = rawOld;
    eepromJson[ss.str()]["rawNew"] = rawNew;
    eepromJson[ss.str()]["comment"] = comment;

    this->saveChanges();
}

void JsonFile::saveChanges() {
    ofstream jsonFileStreamWrite("data/eeprom.json", ios::out);
    styledStream.write(jsonFileStreamWrite, eepromJson);
}
