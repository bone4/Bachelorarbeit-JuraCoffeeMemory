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
