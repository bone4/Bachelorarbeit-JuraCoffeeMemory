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

#include "Storage.hpp"

using namespace std;

Storage::Storage() {
    //ctor
}

void Storage::setParams(string cmd, int row_length, int rows, string filePath) {
    command = cmd;
    storage_row_length = row_length;
    storage_rows = rows;
    length = cmd.size() + row_length;
    logFilePath = filePath;
}

Storage::~Storage()
{
    //dtor
}

void Storage::printRawVector() {
    for (auto &row : raw) {
        cout << string(row) << endl;
    }
    return;
}

string Storage::getRawData() {
    stringstream ss;
    for (auto &row : raw) {
        ss << string(row);
    }
    return ss.str();
}

void Storage::printVector(vector<int> const &v) {
    for (auto &it : v) {
        printf("%d\t%02X\t\n", it, it);
//        cout << dec << it << "\t";
//        cout << hex << it << dec << "\t" << "size: " << v.size() << endl;
    }
    return;
}

void Storage::printByteVector() {
    for (auto &it : bytes) {
        printf("%d\t%02X\t\n", it, it);
//        cout << dec << it << "\t";
//        cout << hex << it << dec << "\t" << "size: " << v.size() << endl;
    }
    return;
}

void Storage::printByteVectorShort() {
    for (auto &it : bytes) {
        printf("%02X", it);
    }
    printf("\n");
    return;
}

int Storage::char2int(char input)
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
void Storage::hexString2int(string src, vector<int> &target) {
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

void Storage::readStorage() {
    // Optimize and search at the right place: RE:xx
/*
    readStorage(0);
    readStorage(16);
    readStorage(32);
    readStorage(48);
*/
    int ende = 256; int i;
    for (i = 0; i < ende; i+=storage_rows) { // from 0x00 to 0xFF
        progressBar(i, ende, "Scanning");
        readStorage(i);
    }
    progressBar(i, ende, "Scanning");
}

void Storage::readStorage(int i) {
    SerialConnection& s = SerialConnection::getInstance();

    stringstream ss;
    ss << command;
    ss << setfill('0') << setw(2) << hex << uppercase << i;


    s.send(ss.str());
    //cout << "Input:\t" << ss.str() << endl;

    string resp = s.receive();
    string data = resp.substr(command.length()); // cut e.g. "rt:" off
    data = removeSpecialCharFromString(data); // remove chars e.g. '\r'

    raw.push_back(data);
    //cout << "Output:\t" << data << endl;

/*
    fstream f;
    f.open(device, fstream::in | fstream::out);

    cout << "Eingabe:\t";
    cout << command;
    cout << setfill('0') << setw(2) << hex << uppercase << i;
    cout << "\r\n";

    f << command;
    f << setfill('0') << setw(2) << hex << uppercase << i;
    f << "\r\n";
    //usleep(3000000);

    char* buffer = new char[length];
    f.read (buffer,length);
    raw.push_back(buffer+command.size());
    cout << "Ausgabe:\t" << string(buffer) << endl;

    cout << endl;

    f.close();
*/
}

void Storage::diffBytesWith(Storage* last, vector<int> excludeBytes, bool writeIntoJsonFile, string comment) {
    JsonFile& jf = JsonFile::getInstance();

    bool hit = false;

    std::string note;
    if (comment == "") {
        cout << COLOR_FG_green << "Say what you've changed:" << COLOR_reset << " ";
        getline(cin, note);
    } else {
        note = comment;
    }

    auto first = last->bytes.begin();
    auto second = this->bytes.begin();
    for (int i = 0; i < (storage_rows*storage_row_length/2); i++) { // rows * (colums/2), 2 hex = 1 Byte
        // int i is not in the array of excudeBytes[]! (after find, the iterator is at the end)
        if (find(excludeBytes.begin(), excludeBytes.end(), i) == excludeBytes.end()) {
            // difference between last and this at position i detected
            if (*(first+i) != *(second+i)) {
                // difference detected
                hit = true;
                cout << "old value: " << *(first+i) << "\t new value:" << *(second+i) << "\t at position: " << i;
                printf(" / %02X", i);
                int group = floor(i/2);
                cout << "\t in 2 byte word no.: " << group;
                printf(" / %02X\n", group);

                stringstream info;
                info << *(first+i) << " -> " << *(second+i) << ": " << note;
                if (writeIntoJsonFile) {
                    jf.logData(logFilePath, i, info.str());
                    jf.logRawData(logFilePath, last->getRawData(), this->getRawData(), note);
                } else {
                    // Write the evaluation to the end of the JSON files (-> data)
                    //jf.logData(logFilePath, i, info.str());
                }
            }
        }
    }
    if (!hit) {
        cout << "Sorry, no Bytes are changed ;(" << endl;
        //last->printByteVectorShort();
        this->printByteVectorShort();
    }
}

void Storage::progressBar(int num, int ende, std::string txt){
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int width = w.ws_col; // Zeilen: w.ws_row; Spalten: w.ws_col
	width -= 8; // konstanter Text, sowie die Prozentausgabe
	width -= txt.length(); // Text

	float progress = (float)num / (float)ende;

	cout << "\r" << txt << " [";
	for (float i=1; i<=width; i++) {
		if ((i/width) <= progress) cout << "#";
		else cout << "-";
	}
	int progress_percent = (int)(progress*100);
	cout << "] " << progress_percent << "%" << flush;
//	cout << "\r" << num << " / " << end << " = " << progress_percent << flush;
}

string Storage::removeSpecialCharFromString(string& txt) {
    // removes the characters '\r' and '\n' from the given string
    txt.erase( remove(txt.begin(), txt.end(), '\r'), txt.end() );
    txt.erase( remove(txt.begin(), txt.end(), '\n'), txt.end() );
    return txt;
}

bool Storage::getBit(unsigned char byte, int position) {
    return (byte >> position) & 0x1;
}
