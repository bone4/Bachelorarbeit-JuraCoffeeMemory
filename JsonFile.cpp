#include "JsonFile.hpp"

JsonFile::JsonFile()
{
    //ctor
}

JsonFile& JsonFile::getInstance() {
    // Singleton Design Pattern
    return instance;
}

// call constructor
JsonFile JsonFile::instance;

JsonFile::~JsonFile()
{
    //dtor
}

Json::Value JsonFile::readJson(string filePath) {
    ifstream jsonFileStreamRead(filePath);
    bool b = reader.parse(jsonFileStreamRead, jsonObj);
    if (!b) {
        cerr << "Error: " << reader.getFormattedErrorMessages();
        exit(9);
    } else {
        return jsonObj;
    }
}

void JsonFile::logData(string filePath, int i, string s) {
    readJson(filePath);

    stringstream ss, data;
    ss << setw(3) << setfill('0') << i;

    data << jsonObj["data"][ss.str()].asString() << s << "###";
    jsonObj["data"][ss.str()] = data.str();

    this->saveChanges(filePath);
}

void JsonFile::logRawData(string filePath, string rawOld, string rawNew, string comment) {
    readJson(filePath);

    auto t = time(nullptr);
    auto tm = *localtime(&t);
    stringstream ss;
    ss << put_time(&tm, "%Y-%m-%d %H-%M-%S");
    jsonObj[ss.str()]["rawOld"] = rawOld;
    jsonObj[ss.str()]["rawNew"] = rawNew;
    jsonObj[ss.str()]["comment"] = comment;

    this->saveChanges(filePath);
}

void JsonFile::saveChanges(string filePath) {
    ofstream jsonFileStreamWrite(filePath, ios::out);
    styledStream.write(jsonFileStreamWrite, jsonObj);
}

vector<dump> JsonFile::getDumps() {
    vector<dump> ret;
    for(Json::Value::const_iterator it = jsonObj.begin(); it != jsonObj.end(); it++) {
        string key = it.key().asString();
        if (key!="timestamp" && key!="data") {
            ret.push_back(dump(
                it.key().asString(), (*it)["comment"].asString(), (*it)["rawOld"].asString(), (*it)["rawNew"].asString()
            ));
        }
    }
    return ret;
}

vector<data> JsonFile::getData() {
    vector<data> ret;
    for(Json::Value::const_iterator it = jsonObj["data"].begin(); it != jsonObj["data"].end(); it++) {
        string byteString = it.key().asString();
        vector<string> comments;
        string CommentString = it->asString();
        string delimiter = "###";

        size_t pos = 0;
        string token;
        while ((pos = CommentString.find(delimiter)) != string::npos) {
            token = CommentString.substr(0, pos);
            comments.push_back(token);
            CommentString.erase(0, pos + delimiter.length());
        }
        comments.push_back(token);

        try {
            ret.push_back(data(
                stoi(byteString), comments
            ));
        } catch (...) {} // skip this one if the byte number can't be converted to an integer
    }
    return ret;
}
