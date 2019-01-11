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
