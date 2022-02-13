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

#include<iostream>
#include<sstream>
#include<iterator>
#include<vector>

using namespace std;












int char2int(char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  throw invalid_argument("Invalid input string");
}

// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters
void hexString2int(string src, vector<int> &target) {
  if ((src.length()%2)==0) {
    for (unsigned int i = 0; i < src.length(); i+=2) {
      target.push_back(
          char2int( src.at(i)   ) * 16
        + char2int( src.at(i+1) )
      );
    }
  } else {
    cerr << "invalid string: " << src << endl;
  }
/*
  while(*ss && ss[1])
  {
    target.push_back(char2int(*ss)*16 + char2int(ss[1]));
    ss += 2;
  }
*/
}




















int main() {
    cout << "Dump: ";

    string input_string;
    getline(cin, input_string);

    if (input_string.find_first_not_of("0123456789ABCDEF") != string::npos) {
        cout << "Invalid chars! Only HEX chars are allowed (0-F). Exit!" << endl;
    }

    if (input_string.length() == 1024 || input_string.length() == 512) {
        // EEPROM || RAM

        // 2 HEX = 1 Byte
        //int new_length = input_string.length() / 2;

        vector<int> bytes;
        hexString2int(input_string, bytes);

        for (int i=0; i<=255; i++) { // 0x00 - 0xFF
            if (i % 16 == 0) {
                cout << endl
                     << "Position\t| Data" << endl
                     << "HEX\tDec.\t| HEX\tDec." << endl;
            }
            printf("0x%02X", i);
            cout << "\t" << i;

            if (input_string.length() == 1024) {
                cout << "\t| " << "0x" << input_string.substr(4*i, 2) << "\t" << bytes[2*i] << endl;
                cout << "\t\t| " << "0x" << input_string.substr(4*i+2, 2) << "\t" << bytes[2*i+1] << endl;
            }
            if (input_string.length() == 512) {
                cout << "\t| " << "0x" << input_string.substr(2*i, 2) << "\t" << bytes[i] << endl;
            }
        }
    } else {
        cout << "Invalid length! The dump must have 1024 chars for EEPROM or 512 chars for RAM. Exit!" << endl;
        return 2;
    }

    return 0;
}
