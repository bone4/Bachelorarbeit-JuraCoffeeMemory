#ifndef RAM_H
#define RAM_H

#include <string>
#include "Storage.hpp"

class RAM : public Storage
{
    public:
        RAM(bool offline, string filePath);
        virtual ~RAM();

    protected:

    private:
        const std::string RAM_command = "RR:";
        const int RAM_row_length = 32; // 32 hex chars in 1 row
        const int RAM_rows = 16; // 16 rows for whole RAM content
        static int run; // Run number
};

#endif // RAM_H
