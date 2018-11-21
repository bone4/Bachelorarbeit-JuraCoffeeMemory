#ifndef EEPROM_H
#define EEPROM_H

#include <string>
#include "Storage.hpp"

class EEPROM : public Storage
{
    public:
        EEPROM(bool offline, string filePath);
        virtual ~EEPROM();

    protected:

    private:
        const std::string EEPROM_command = "RT:";
        const int EEPROM_row_length = 64; // 64 hex chars in 1 row
        const int EEPROM_rows = 16; // 16 rows for whole EEPROM content
        static int run; // Run number
};

#endif // EEPROM_H
