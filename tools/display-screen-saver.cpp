#include <iostream>
#include <sstream>
#include <unistd.h>
#include "../SerialConnection.hpp"

#define DISPLAY_SIZE 8

using namespace std;

int main() {
	SerialConnection& s = SerialConnection::getInstance();
	s.connect();

	string text1 = "HALLO,";
	string text2 = "BILDSCHIRM-SCHONER: HIER K$NNTE IHRE WERBUNG STEHEN... %";

	string text1AtTheEnd = " I LOVE";
	string text2AtTheEnd = " TURAU %";

	unsigned int microseconds;
	stringstream ss1, ss2;
	string txt1, txt2;
	string resp1, resp2;

	txt1 = text1.substr(0, DISPLAY_SIZE); // take visible part
	txt1.append(DISPLAY_SIZE - txt1.length(), ' '); // fill up with spaces
	ss1 << "?D1" << txt1;
	cout << "Command 1: " << ss1.str();
	s.send(ss1.str());
	resp1 = s.receive();
	cout << "\tResponse 1: " << resp1 << endl;

//	while (true) {
		for (unsigned int i=0; i<=((text1.length() > text2.length()) ? (text1.length()) : (text2.length())); i++) {
//			if (i <= text1.length()) {
//				txt1 = text1.substr(i, DISPLAY_SIZE); // take visible part
//			} else {
//				txt1 = "";
//			}
			if (i <= text2.length()) {
				txt2 = text2.substr(i, DISPLAY_SIZE);
			} else {
				txt2 = "";
			}
//			txt1.append(DISPLAY_SIZE - txt1.length(), ' '); // fill up with spaces
			txt2.append(DISPLAY_SIZE - txt2.length(), ' ');

//			ss1.str(string()); // clear stringstream
//			ss1 << "?D1" << txt1;
			ss2.str(string()); // clear stringstream
			ss2 << "?D2" << txt2;

//			cout << "Command 1: " << ss1.str();
//			s.send(ss1.str());
//
//			resp1 = s.receive();
//			cout << "\tResponse 1: " << resp1 << endl;

			cout << "Command 2: " << ss2.str();
			s.send(ss2.str());

			resp2 = s.receive();
			cout << "\tResponse 2: " << resp2 << endl;

			if (i==0) {
				microseconds = 2000000; // = 2s
			} else {
				microseconds = 0; // = ~0.1s
			}
			usleep(microseconds);
		}
//	}

	txt1 = text1AtTheEnd.substr(0, DISPLAY_SIZE); // take visible part
	txt1.append(DISPLAY_SIZE - txt1.length(), ' '); // fill up with spaces
	ss1.str(string()); // clear stringstream
	ss1 << "?D1" << txt1;
	cout << "Command 1: " << ss1.str();
	s.send(ss1.str());
	resp1 = s.receive();
	cout << "\tResponse 1: " << resp1 << endl;

	txt2 = text2AtTheEnd.substr(0, DISPLAY_SIZE); // take visible part
	txt2.append(DISPLAY_SIZE - txt2.length(), ' '); // fill up with spaces
	ss2.str(string()); // clear stringstream
	ss2 << "?D2" << txt2;
	cout << "Command 2: " << ss2.str();
	s.send(ss2.str());
	resp2 = s.receive();
	cout << "\tResponse 2: " << resp2 << endl;

	s.disconnect();
	return 0;
}
