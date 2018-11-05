#ifndef RAM_H
#define RAM_H

#include <string>
#include "Storage.hpp"
#include "JsonFile.hpp"

class RAM : public Storage
{
    public:
        RAM(bool offline, string filePath);
        virtual ~RAM();

        void diffBytesWith(RAM* last);

    protected:

    private:
        const int RAM_row_length = 32; // 32 hex chars in 1 row
        const int RAM_rows = 16; // 16 rows for whole RAM content
        static int run; // Run number

        string logFilePath;
};

#endif // RAM_H
