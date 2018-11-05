#ifndef JSONFILE_H
#define JSONFILE_H

#include <jsoncpp/json/json.h>
#include <iostream> // cerr
#include <fstream> // ifstream & ofstream
#include <sstream> // stringstream
#include <iomanip> // setw(), leading zeros in int->string
#include <ctime>

using namespace std;

class JsonFile
{
    public:
        // Singleton Design Pattern
        static JsonFile& getInstance();

        virtual ~JsonFile();

        void eepromData(string filePath, int i, string s);
        void eepromLog(string filePath, string rawOld, string rawNew, string comment);

    protected:

    private:
        // Singleton Design Pattern
        JsonFile();
        static JsonFile instance;
        // Copy constructor forbidden
        JsonFile(const JsonFile& other);
        // Assignment operator forbidden
        JsonFile operator=(JsonFile& other);

        ifstream jsonFileStreamReadEEPROM;
        Json::Reader reader;
        Json::Value eepromJson;
        Json::StyledStreamWriter styledStream;

        void saveChanges(string filePath);
};

#endif // JSONFILE_H
