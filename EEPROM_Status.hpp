/*
 * Copyright (c) 2019 Niklas Krüger <niklas.krueger@tuhh.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
