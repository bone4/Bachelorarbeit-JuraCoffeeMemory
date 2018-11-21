#ifndef JSONFILE_H
#define JSONFILE_H

#include <jsoncpp/json/json.h>
#include <iostream> // cerr
#include <fstream> // ifstream & ofstream
#include <sstream> // stringstream
#include <iomanip> // setw(), leading zeros in int->string
#include <ctime>
#include <vector>

using namespace std;

// A Dump consists of a timestamp, comment, raw-data at begin and raw-data at the end
struct dump {
    string timestamp;
    string comment;
    string rawOld;
    string rawNew;
    dump(string t, string c, string rOld, string rNew) :
        timestamp(t), comment(c), rawOld(rOld), rawNew(rNew)
        {}
};
// A data entry has an index byte number and a list of comments
struct data {
    int byte;
    vector<string> comments;
    data(int b, vector<string> c) :
        byte(b), comments(c)
        {}
};

class JsonFile
{
    public:
        // Singleton Design Pattern
        static JsonFile& getInstance();

        virtual ~JsonFile();

        Json::Value readJson(string filePath);

        void logData(string filePath, int i, string s);
        void logRawData(string filePath, string rawOld, string rawNew, string comment);

        vector<dump> getDumps();
        vector<data> getData();

    protected:

    private:
        // Singleton Design Pattern
        JsonFile();
        static JsonFile instance;
        // Copy constructor forbidden
        JsonFile(const JsonFile& other);
        // Assignment operator forbidden
        JsonFile operator=(JsonFile& other);

        Json::Reader reader;
        Json::Value jsonObj;
        Json::StyledStreamWriter styledStream;

        void saveChanges(string filePath);
};

#endif // JSONFILE_H
