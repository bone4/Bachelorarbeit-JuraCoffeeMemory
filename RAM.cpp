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

#include "RAM.hpp"

using namespace std;

int RAM::run = 0;

RAM::RAM(bool offline, string filePath)
{
    //ctor
    setParams(RAM_command, RAM_row_length, RAM_rows, filePath); // read 1 row RAM wich consits of 32 hex chars

    work_offline = offline;

    if (!work_offline) { // ONLINE
        readStorage();
    } else { // OFFLINE

    }



    for (auto &row : raw) {
        hexString2int(row, bytes);
    }
}

RAM::~RAM()
{
    //dtor
}
