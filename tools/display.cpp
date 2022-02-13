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

#include <iostream>
#include <sstream>
#include <unistd.h>
#include "../SerialConnection.hpp"

using namespace std;

int main() {
	SerialConnection& s = SerialConnection::getInstance();
	s.connect();

	for (int i=32; i<=96; i++) {
		char buffer [10];
		sprintf(buffer, "%03d %c", i, i);

		stringstream ss;
		ss << "?D2" << buffer << "   ";
		
		cout << "Command: " << ss.str();
		s.send(ss.str());

		string resp;
		resp = s.receive();
		cout << "\tResponse: " << resp << endl;

		unsigned int microseconds = 100000; // = 0.1s
		usleep(microseconds);
	}

	s.disconnect();
	return 0;
}
