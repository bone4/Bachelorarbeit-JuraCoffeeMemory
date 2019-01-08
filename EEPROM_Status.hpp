#ifndef EEPROM_STATUS_H
#define EEPROM_STATUS_H

#include <string>
#include <set>
#include "Storage.hpp"
#include "SerialConnection.hpp"
#include <jsoncpp/json/json.h>

struct EntryEEPROM {
    int word;       // address 0x00 - 0xFF
    set<int> bytes; // 2x: 0-255
    string label;   // text

    bool operator<(const EntryEEPROM &other) const { return label < other.label; }
};

class EEPROM_Status : public Storage
{
    public:
        EEPROM_Status();
        virtual ~EEPROM_Status();

        string get_EEPROM_Status();
        void pretty_print_json();

        bool write_EEPROM();

    protected:

    private:
        string writeCommand;
        string writeCommandAnswer;

        void scan(); // read all relevant status information

        set<int> known_bytes; // only this bytes read from the coffee machine

        set<EntryEEPROM> getEntriesEEPROM(); // table of position and label from known properties

        Json::Value jsonObj;
        // Json::StyledStreamWriter styledStream; // triggered by operator<<
        Json::FastWriter fast;
};

#endif // EEPROM_STATUS_H
