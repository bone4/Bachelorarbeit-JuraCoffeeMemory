#ifndef RAM_STATUS_H
#define RAM_STATUS_H

#include <string>
#include <set>
#include "Storage.hpp"
#include "SerialConnection.hpp"
#include <jsoncpp/json/json.h>
#include <math.h> /* pow */

struct EntryRAM {
    int byte;       // address 0x00 - 0xFF
    set<int> bits;  // 8x: 0 or 1
    string label;   // text

    bool operator<(const EntryRAM &other) const { return label.compare(other.label); }
};

class RAM_Status : public Storage
{
    public:
        RAM_Status();
        virtual ~RAM_Status();

        string get_RAM_Status();
        void pretty_print_json();

    protected:

    private:
        void scan(); // read all relevant status information

        set<int> known_bytes; // only this bytes read from the coffee machine

        set<EntryRAM> getEntriesRAM(); // table of position and label from known properties

        Json::Value jsonObj;
        // Json::StyledStreamWriter styledStream; // triggered by operator<<
        Json::FastWriter fast;
};

#endif // RAM_STATUS_H
