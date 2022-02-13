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

#ifndef EEPROM_H
#define EEPROM_H

#include <string>
#include "Storage.hpp"

class EEPROM : public Storage
{
    public:
        EEPROM(bool offline, string filePath);
        virtual ~EEPROM();

    protected:

    private:
        const std::string EEPROM_command = "RT:";
        const int EEPROM_row_length = 64; // 64 hex chars in 1 row
        const int EEPROM_rows = 16; // 16 rows for whole EEPROM content
        static int run; // Run number
};

#endif // EEPROM_H
