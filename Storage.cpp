#include "Storage.hpp"

using namespace std;

Storage::Storage(string c, int l) :
    command(c),
    length(c.size() + l) // response = command + data
{
    //ctor
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

string Storage::removeSpecialCharFromString(string& txt) {
    // removes the characters '\r' and '\n' from the given string
    txt.erase( remove(txt.begin(), txt.end(), '\r'), txt.end() );
    txt.erase( remove(txt.begin(), txt.end(), '\n'), txt.end() );
    return txt;
}
