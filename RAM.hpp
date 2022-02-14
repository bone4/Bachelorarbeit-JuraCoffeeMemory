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

#ifndef RAM_H
#define RAM_H

#include <string>
#include "Storage.hpp"

class RAM : public Storage
{
    public:
        RAM(bool offline, string filePath);
        virtual ~RAM();

    protected:

    private:
        const std::string RAM_command = "RR:";
        const int RAM_row_length = 32; // 32 hex chars in 1 row
        const int RAM_rows = 16; // 16 rows for whole RAM content
        static int run; // Run number
};

#endif // RAM_H
