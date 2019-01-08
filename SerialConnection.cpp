#include "SerialConnection.hpp"

SerialConnection::SerialConnection() :
    testCommand("TY:"),
    testAnswer("ty:E1300 CAPU 3\r"),
    serialPort("/dev/ttyACM0"),
    commandSuffix("\r\n")
{
    //ctor
//    input_buffer = new char[BUFFER_SIZE];
}

SerialConnection& SerialConnection::getInstance() {
    // Singleton Design Pattern
    return instance;
}

// call constructor
SerialConnection SerialConnection::instance;

SerialConnection::~SerialConnection()
{
    //dtor
//    delete[] input_buffer;
    this->disconnect();
}

void SerialConnection::setPort(string port) {
    serialPort = port;
}

string SerialConnection::getPort() {
    return serialPort;
}

void SerialConnection::setCommandSuffix(string suffix) {
    commandSuffix = suffix;
}

string SerialConnection::getCommandSuffix() {
    return commandSuffix;
}

void SerialConnection::connect() {
    // lock device file to this process
    int fd = open(serialPort.c_str(), O_RDWR, 0666); // open devicefile/lockfile
    int rc = flock(fd, LOCK_EX | LOCK_NB); // grab exclusive lock, fail if can't obtain.
    if (rc) {
        cerr << "You must wait! Another process is using " << serialPort << ", or it doesn't exist." << endl;
		exit(255);
    }

    // Open the hardware serial stream.
    serial_stream.Open( serialPort );
    if (!serial_stream.good()) {
        std::cerr << COLOR_BG_red << "[" << __FILE__ << ":" << __LINE__ << "]" << COLOR_reset
            << COLOR_FG_red
            << " Error: Could not open serial port: "
            << serialPort
            << COLOR_reset
            << std::endl;
        exit(3);
    }

    // Configure the stream
    serial_stream.SetBaudRate( LibSerial::SerialStreamBuf::BAUD_9600 );
//    serial_stream.SetCharSize( LibSerial::SerialStreamBuf::CHAR_SIZE_8 );
//    serial_stream.SetFlowControl( LibSerial::SerialStreamBuf::FLOW_CONTROL_NONE );
//    serial_stream.SetNumOfStopBits(1);
//    serial_stream.SetParity( LibSerial::SerialStreamBuf::PARITY_NONE );
    serial_stream.SetVMin(BUFFER_SIZE);
    serial_stream.SetVTime(1);
    if (!serial_stream.good()) {
        std::cerr << COLOR_BG_red << "[" << __FILE__ << ":" << __LINE__ << "]" << COLOR_reset
            << COLOR_FG_red
            << " Error: Could not configure serial port."
            << COLOR_reset
            << std::endl;
        exit(4);
    }

    if (!testConnection()) {
        exit(5);
    }
}

void SerialConnection::disconnect() {
    // Close the hardware serial stream.
    serial_stream.Close();

    // unlock device file to other processes
    int fd = open(serialPort.c_str(), O_RDWR, 0666); // open devicefile/lockfile
    flock(fd, LOCK_UN); // Remove an existing lock held by this process.
}

bool SerialConnection::testConnection() {
    send(testCommand);
    string answer = receive();

    if (testAnswer != answer) {
        cerr << COLOR_BG_red << "[" << __FILE__ << ":" << __LINE__ << "] Test command failed:" << COLOR_reset << endl;
        cerr << COLOR_FG_red << answer << COLOR_reset << endl
             << COLOR_BG_red << "is not" << COLOR_reset << endl
             << COLOR_FG_red << testAnswer << COLOR_reset << endl;
        return false;
    } else {
        return true;
    }
}
void SerialConnection::setTestCommand(string command) {
    testCommand = command;
}
string SerialConnection::getTestCommand() {
    return testCommand;
}
void SerialConnection::setTestAnswer(string answer) {
    testAnswer = answer;
}
string SerialConnection::getTestAnswer() {
    return testAnswer;
}

void SerialConnection::send(string txt) {
    // Send data
    serial_stream << removeSpecialCharFromString(txt) << commandSuffix;
}

void ALARMhandler(int sig) {
    (void)sig;

    SerialConnection& s = SerialConnection::getInstance();

    cerr << COLOR_BG_red << "[" << __FILE__ << ":" << __LINE__ << "]" << COLOR_reset
         << COLOR_FG_red << "Timeout: No response within 3 seconds. If the last command was right, try:" << COLOR_reset << endl
         << "$ " << COLOR_FG_blue << "screen " << s.getPort() << " 9600" << COLOR_reset << endl
         << COLOR_FG_red << "or run this script again." << COLOR_reset << endl;
    exit(2);
}

string SerialConnection::receive() {
    // Wait max 3 secounds
    alarm(3);
    signal(SIGALRM, ALARMhandler);

    // Receive data
    string input_string;
    getline(serial_stream, input_string);

    // Cancel alarm
    alarm(0);

//    serial_stream.read( input_buffer,
//                        BUFFER_SIZE ) ;
//    return input_buffer;
    return input_string;
}

string SerialConnection::removeSpecialCharFromString(string& txt) {
    // removes the characters '\r' and '\n' from the given string
    txt.erase( remove(txt.begin(), txt.end(), '\r'), txt.end() );
    txt.erase( remove(txt.begin(), txt.end(), '\n'), txt.end() );
    return txt;
}
