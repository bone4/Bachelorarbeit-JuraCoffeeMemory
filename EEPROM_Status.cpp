/*
 * Copyright (c) 2019 Niklas Krüger <niklas.krueger@tuhh.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "EEPROM_Status.hpp"

using namespace std;

EEPROM_Status::EEPROM_Status() :
    writeCommand("WE:"),
    writeCommandAnswer("ok:")
{
    //ctor
}

EEPROM_Status::~EEPROM_Status()
{
    //dtor
}

void EEPROM_Status::scan() {
    SerialConnection& s = SerialConnection::getInstance();
    s.connect();



    // 0x00 - 0x0F
    for (int i=0; i<=15; i++) { known_bytes.insert(i); }
    s.send("RT:00");
    string resp1 = s.receive();
    string data1 = resp1.substr(3); // cut e.g. "rt:" off
    data1 = removeSpecialCharFromString(data1); // remove chars e.g. '\r'
    raw.push_back(data1);

    // 0x10
    raw.push_back("0000");

    // 0x11
    known_bytes.insert(17);
    s.send("RE:11");
    string resp2 = s.receive();
    string data2 = resp2.substr(3); // cut e.g. "re:" off
    data2 = removeSpecialCharFromString(data2); // remove chars e.g. '\r'
    raw.push_back(data2);

    // 0x12 - 0x23
    for (int i=18; i<=35; i++) { raw.push_back("0000"); }

    // 0x24 - 0x33
    for (int i=36; i<=51; i++) { known_bytes.insert(i); }
    s.send("RT:24");
    string resp3 = s.receive();
    string data3 = resp3.substr(3); // cut e.g. "rt:" off
    data3 = removeSpecialCharFromString(data3); // remove chars e.g. '\r'
    raw.push_back(data3);

    // 0x34
    known_bytes.insert(52);
    s.send("RE:34");
    string resp4 = s.receive();
    string data4 = resp4.substr(3); // cut e.g. "re:" off
    data4 = removeSpecialCharFromString(data4); // remove chars e.g. '\r'
    raw.push_back(data4);

    // 0x35 - 0x73
    for (int i=53; i<=115; i++) { raw.push_back("0000"); }

    // 0x74
    known_bytes.insert(116);
    s.send("RE:74");
    string resp5 = s.receive();
    string data5 = resp5.substr(3); // cut e.g. "re:" off
    data5 = removeSpecialCharFromString(data5); // remove chars e.g. '\r'
    raw.push_back(data5);

    // 0x75 - 0x7C
    for (int i=117; i<=124; i++) { raw.push_back("0000"); }

    // 0x7D - 0x8C
    for (int i=125; i<=140; i++) { known_bytes.insert(i); }
    s.send("RT:7D");
    string resp6 = s.receive();
    string data6 = resp6.substr(3); // cut e.g. "rt:" off
    data6 = removeSpecialCharFromString(data6); // remove chars e.g. '\r'
    raw.push_back(data6);

    // 0x8D - 0xAF
    for (int i=141; i<=175; i++) { raw.push_back("0000"); }

    // 0xB0 - 0xBF
    for (int i=176; i<=191; i++) { known_bytes.insert(i); }
    s.send("RT:B0");
    string resp7 = s.receive();
    string data7 = resp7.substr(3); // cut e.g. "rt:" off
    data7 = removeSpecialCharFromString(data7); // remove chars e.g. '\r'
    raw.push_back(data7);

    // 0xC0 - 0xFF
    for (int i=192; i<=255; i++) { raw.push_back("0000"); }



    s.disconnect();

    for (auto &row : raw) {
        hexString2int(row, bytes);
    }
}

string EEPROM_Status::get_EEPROM_Status() {
    set<EntryEEPROM> table = getEntriesEEPROM();

    // 1. check if raw is not empty -> call scan()
    scan();
//    for (auto i : known_bytes) {
//        cout << to_string(i) << " ";
//    }
//    cout << endl;
//    printRawVector();

    // 2. check if "std::vector<int> bytes" contains of exactly 512 Bytes
    if (bytes.size() != 512) {
        cerr << "Information of 512 bytes EEPROM are needed, but " << to_string(bytes.size()) << " are given." << endl;
        exit(8);
    }

    for (auto ent : table) {
        // 3. check if getEntriesRAM.bytes are in known_bytes (safety)
        if (known_bytes.find(ent.word) == known_bytes.end()) {
            cerr << "Word " << to_string(ent.word) << " in EEPROM_Status::getEntriesEEPROM() is not known from EEPROM_Status::scan()!" << endl;
            exit(9);
        }

        // 4. loop over getEntriesRAM, extract the bits and build the json object
        if (ent.bytes.size() > 1) {                         // {0,1}
            jsonObj[ent.label]["value"] = bytes[2*(ent.word)]*256 + bytes[2*(ent.word) + 1];
        } else if (ent.bytes.find(0) != ent.bytes.end()) {  // {0}
            jsonObj[ent.label]["value"] = bytes[2*(ent.word)];
        } else if (ent.bytes.find(1) != ent.bytes.end()) {  // {1}
            jsonObj[ent.label]["value"] = bytes[2*(ent.word) + 1];
        } else {
            jsonObj[ent.label]["value"] = "ERROR";
        }
    }

    // 5. return json object as short string
    return fast.write(jsonObj);
}
void EEPROM_Status::pretty_print_json() {
    cout << jsonObj << endl; // Json::StyledStreamWriter for operator<<
}

bool EEPROM_Status::write_EEPROM() {
    SerialConnection& s = SerialConnection::getInstance();

    set<EntryEEPROM> table = getEntriesEEPROM();

    Json::Value val;
    Json::Reader reader;
    bool b = reader.parse(cin, val);
    if (!b) {
        cerr << "Error: " << reader.getFormattedErrorMessages();
        exit(10);
    } else {
        s.connect();

//        cout << val << endl;
        if (val.type() == Json::objectValue) {
            vector<string> keys = val.getMemberNames();
            for (size_t i=0; i<keys.size(); i++) { // iterate over all key labels
                const string& key = keys[i];
                EntryEEPROM searchElem; searchElem.label = key;
                set<EntryEEPROM>::iterator itTable = table.find(searchElem);
                if (                                                // check if
                    val[key].isMember("value") &&                   // subkey "value" exists and if
                    (val[key]["value"].type() == Json::intValue) && // the value is an interger value and if
                    (itTable != table.end())                        // the key is a known key from getEntriesEEPROM()
                ) {
//                    // Debug
//                    cout << "\"" << key << "\": " << val[key]["value"]
//                         << " => corresponding entry: " << itTable->label
//                         << ", word: " << to_string(itTable->word) << endl;

                    stringstream ss;
                    stringstream writeAddress;
                    writeAddress << setfill('0') << setw(2) << hex << uppercase << (itTable->word);
                    ss << writeCommand << writeAddress.str() << ",";

                    int newValue = val[key]["value"].asInt();

                    if (itTable->bytes.size() > 1) {
                        // whole word

                        if ((newValue < 0) || (newValue > 65535)) {
                            cerr << "new value for '" << key << "' is out of range!" << endl;
                            s.disconnect();
                            exit(13);
                        }

                        ss << setfill('0') << setw(4) << hex << uppercase << newValue;
                    } else if (itTable->bytes.size() == 1) {
                        // read first and replace only the correct parts

                        if ((newValue < 0) || (newValue > 255)) {
                            cerr << "new value for '" << key << "' is out of range!" << endl;
                            s.disconnect();
                            exit(14);
                        }

                        for (auto byte : itTable->bytes) {
                            stringstream tmp;
                            tmp << "RE:" << writeAddress.str();
                            s.send(tmp.str());
                            string resp1 = s.receive();

                            if (byte == 0) {
                                string data1 = resp1.substr(5, 2); // take * from "re:xx**\r" and add xx as new value
                                ss << setfill('0') << setw(2) << hex << uppercase << newValue
                                   << data1;
                            } else if (byte == 1) {
                                string data1 = resp1.substr(3, 2); // take * from "re:**xx\r" and add xx as new value
                                ss << data1
                                   << setfill('0') << setw(2) << hex << uppercase << newValue;
                            } else {
                                cerr << "Please enter only the bytes 0 or 1 in EEPROM_Status::getEntriesEEPROM()!" << endl;
                                s.disconnect();
                                exit(12);
                            }
                        }
                    } // else ignore the faulty bytes fild

                    s.send(ss.str());
//                    cout << "Command:\t" << ss.str() << endl;

                    string resp = s.receive();
                    string data = removeSpecialCharFromString(resp); // remove chars e.g. '\r'
                    if (data != writeCommandAnswer) {
                        cerr << "Error: " << data << "###";
                        s.disconnect();
                        exit(11);
                    } else {
                        cout << data << " " << key << "###";
                    }
                }
            }
        }
    }
    s.disconnect();
    return true;
}

set<EntryEEPROM> EEPROM_Status::getEntriesEEPROM() {
    set<EntryEEPROM> ret;

    ret.insert(EntryEEPROM{  0, {0,1}, "amount_1_big_coffee_with_beans"});      // Zähler für 1 große Tasse übers Bohnenfach
    ret.insert(EntryEEPROM{  1, {0,1}, "amount_2_big_coffees_with_beans"});     // Zähler für 2 große Tasse übers Bohnenfach
    ret.insert(EntryEEPROM{  2, {0,1}, "amount_1_small_coffee_with_beans"});    // Zähler für 1 kleine Tasse übers Bohnenfach
    ret.insert(EntryEEPROM{  3, {0,1}, "amount_2_small_coffees_with_beans"});   // Zähler für 2 kleine Tasse übers Bohnenfach
    ret.insert(EntryEEPROM{  4, {0,1}, "amount_special_coffee_with_beans"});    // Zähler für Spezial-Tasse übers Bohnenfach
    ret.insert(EntryEEPROM{  5, {0,1}, "amount_filter_replacements"});          // Zähler für jeden neu eingesetzten Filter
    ret.insert(EntryEEPROM{  6, {0,1}, "amount_pulver_coffees"});               // Zähler für alle Zubereitungen mit Pulver (egal ob 1 oder 2, Zähler wird immer inkrementiert)
    ret.insert(EntryEEPROM{  7, {0,1}, "amount_rinsings"});                     // Spülen Gesammtzähler (wird in der Reinigung nicht zurück auf 0 gesetzt)
    ret.insert(EntryEEPROM{ 13, {0,1}, "amount_calcified"});					// Zähler für die Verkalkung der Maschine; (Meldung kommt nur, wenn kein Filter eingesetzt ist)
    jsonObj["amount_calcified"]["min"] = 0;
    jsonObj["amount_calcified"]["max"] = 794;
    jsonObj["amount_calcified"]["default"] = 0;
    ret.insert(EntryEEPROM{ 14, {0,1}, "amount_ground_1"});                     // Zähler für Trester; (Schale min 8 Sekunden entnehmen setzt den Zähler auf 0 zurück); (0x000F = 15: Kaffee bereit; 0x0010 = 16: Trester leeren)
    jsonObj["amount_ground_1"]["min"] = 0;
    jsonObj["amount_ground_1"]["max"] = 15;
    jsonObj["amount_ground_1"]["default"] = 0;
    ret.insert(EntryEEPROM{ 15, {0,1}, "amount_coffee_preparations_until_next_cleaning"}); // Zähler für alle Zubereitungen (Pulver und Bohnen) (egal ob 1 oder 2, Zähler wird immer inkrementiert); Zähler für Reinigungsankündigung nach Bezügen (bis 0x00DB (219) OK, Meldung ab 0x00DC (220))
    jsonObj["amount_coffee_preparations_until_next_cleaning"]["min"] = 0;
    jsonObj["amount_coffee_preparations_until_next_cleaning"]["max"] = 219;
    jsonObj["amount_coffee_preparations_until_next_cleaning"]["default"] = 0;
    ret.insert(EntryEEPROM{ 17, {0,1}, "amount_rinsings_until_next_cleaning"}); // Spülen; bei jeder Spülung +=1; Zähler für Reinigungsankündigung nach Spülungen (bis 0x00B3 (179) OK, Meldung ab 0x00B4 (180))
    jsonObj["amount_rinsings_until_next_cleaning"]["min"] = 0;
    jsonObj["amount_rinsings_until_next_cleaning"]["max"] = 179;
    jsonObj["amount_rinsings_until_next_cleaning"]["default"] = 0;
    ret.insert(EntryEEPROM{ 36, {0}, "water_hardness"});                        // Wasserhärte --,1,2,3,4
    jsonObj["water_hardness"]["min"] = 0;
    jsonObj["water_hardness"]["max"] = 4;
    ret.insert(EntryEEPROM{ 36, {1}, "economy_mode"});                          // Economy Mode off (0) oder on (1)
    jsonObj["economy_mode"]["min"] = 0;
    jsonObj["economy_mode"]["max"] = 1;
    ret.insert(EntryEEPROM{ 37, {0}, "automatic_switch_on_time_hours"});        // automatische Einschaltzeit, die Stunde 0-23 deaktiviert (reset mit <N>) setzt die Stunde auf 128 => re:8000 (hex)
    jsonObj["automatic_switch_on_time_hours"]["min"] = 0;
    jsonObj["automatic_switch_on_time_hours"]["max"] = 23;
    jsonObj["automatic_switch_on_time_hours"]["disabled"] = 128;
    ret.insert(EntryEEPROM{ 37, {1}, "automatic_switch_on_time_minutes"});      // automatische Einschaltzeit, die Minute 0-59 deaktiviert (reset mit <N>) setzt die Minute auf 0
    jsonObj["automatic_switch_on_time_minutes"]["min"] = 0;
    jsonObj["automatic_switch_on_time_minutes"]["max"] = 59;
    jsonObj["automatic_switch_on_time_minutes"]["disabled"] = 0;
    ret.insert(EntryEEPROM{ 38, {1}, "automatic_switch_off_interval"});         // automatische Ausschaltzeit, 0 deaktiviert, 1 0,5h, 2 1h, 4 2h, 6 3h, 8 4h, 10 5h, 12 6h, 14 7h, 16 8h, 18 9h; Reset [N] stellt auf den Standardwert: 10 (5h)
    jsonObj["automatic_switch_off_interval"]["valid_values"][0] = 0;
    jsonObj["automatic_switch_off_interval"]["valid_values"][1] = 1;
    jsonObj["automatic_switch_off_interval"]["valid_values"][2] = 2;
    jsonObj["automatic_switch_off_interval"]["valid_values"][3] = 4;
    jsonObj["automatic_switch_off_interval"]["valid_values"][4] = 6;
    jsonObj["automatic_switch_off_interval"]["valid_values"][5] = 8;
    jsonObj["automatic_switch_off_interval"]["valid_values"][6] = 10;
    jsonObj["automatic_switch_off_interval"]["valid_values"][7] = 12;
    jsonObj["automatic_switch_off_interval"]["valid_values"][8] = 14;
    jsonObj["automatic_switch_off_interval"]["valid_values"][9] = 16;
    jsonObj["automatic_switch_off_interval"]["valid_values"][10] = 18;
    jsonObj["automatic_switch_off_interval"]["default"] = 10;
    jsonObj["automatic_switch_off_interval"]["disabled"] = 0;
    ret.insert(EntryEEPROM{ 49, {0,1}, "machine_type"});                        // Maschinen Typ, siehe commands.txt
    ret.insert(EntryEEPROM{ 52, {0,1}, "amount_ground_2"});                     // Zähler für Trester; (Schale min 8 Sekunden entnehmen setzt den Zähler auf 0 zurück); (0x03BF = 959: Kaffee bereit; 0x03C0 = 960: Trester leeren)
    jsonObj["amount_ground_2"]["min"] = 0;
    jsonObj["amount_ground_2"]["max"] = 959;
    jsonObj["amount_ground_2"]["default"] = 0;
    ret.insert(EntryEEPROM{116, {1}, "filter"});                                // 0: Filter nein; 1: Filter ja
    jsonObj["filter"]["valid_values"][0] = 0;
    jsonObj["filter"]["valid_values"][1] = 8;
    jsonObj["filter"]["disabled"] = 0;
    ret.insert(EntryEEPROM{125, {0,1}, "amount_machine_starts"});               // Zähler Einschaltvorgänge
    ret.insert(EntryEEPROM{129, {1}, "language"}); // Sprache
    jsonObj["language"]["valid_values"][0] = 0;  // Deutsch
    jsonObj["language"]["valid_values"][1] = 16; // Italienisch
    jsonObj["language"]["valid_values"][2] = 32; // Niederländisch
    jsonObj["language"]["valid_values"][3] = 48; // Spanisch
    jsonObj["language"]["valid_values"][4] = 64; // Englisch
    jsonObj["language"]["valid_values"][5] = 80; // Französisch
    jsonObj["language"]["valid_values"][6] = 96; // Portugisisch
    ret.insert(EntryEEPROM{130, {1}, "powder_quantity_small_coffee"});          // Pulvermenge einer kleinen Tasse
    jsonObj["powder_quantity_small_coffee"]["min"] = 0;
    jsonObj["powder_quantity_small_coffee"]["max"] = 28;
    jsonObj["powder_quantity_small_coffee"]["default"] = 5;
    ret.insert(EntryEEPROM{131, {1}, "powder_quantity_big_coffee"});            // Pulvermenge einer großen Tasse
    jsonObj["powder_quantity_big_coffee"]["min"] = 0;
    jsonObj["powder_quantity_big_coffee"]["max"] = 28;
    jsonObj["powder_quantity_big_coffee"]["default"] = 8;
    ret.insert(EntryEEPROM{132, {1}, "powder_quantity_special_coffee"});        // Pulvermenge einer Spezialtasse
    jsonObj["powder_quantity_special_coffee"]["min"] = 0;
    jsonObj["powder_quantity_special_coffee"]["max"] = 28;
    jsonObj["powder_quantity_special_coffee"]["default"] = 11;
    ret.insert(EntryEEPROM{133, {1}, "temperature"});                           // Temperatur Kaffee
    jsonObj["temperature"]["min"] = 0;
    jsonObj["temperature"]["max"] = 7;                                          // 1 Temperatur hoch große Tasse; 2 Temperatur hoch kleine Tasse; 4 Temperatur hoch Spezialtasse
    jsonObj["temperature"]["default"] = 0;
    ret.insert(EntryEEPROM{134, {0,1}, "water_quantity_small_coffee"});         // Wassermenge einer kleinen Tasse
    jsonObj["water_quantity_small_coffee"]["default"] = 180;
    ret.insert(EntryEEPROM{135, {0,1}, "water_quantity_big_coffee"});           // Wassermenge eienr großen Tasse
    jsonObj["water_quantity_big_coffee"]["default"] = 340;
    ret.insert(EntryEEPROM{136, {0,1}, "water_quantity_special_coffee"});       // Wassermenge einer Spezialtasse
    jsonObj["water_quantity_special_coffee"]["default"] = 380;
    ret.insert(EntryEEPROM{176, {1}, "incasso_mode"});                          // Inkasso Modus
    jsonObj["incasso_mode"]["valid_values"][0] = 0;
    jsonObj["incasso_mode"]["valid_values"][1] = 16;
    jsonObj["incasso_mode"]["disabled"] = 0;
    ret.insert(EntryEEPROM{179, {0,1}, "amount_water"});                        // Spülungen; Beim Filter einlegen auf 0 zurück gesetzt; zählt nur bei eingelegtem Filter; Zähler für Wassermenge => Filter wechseln; (0x01F3 = 499: Kaffee bereit; 0x01F4 = 500: Filter wechseln)
    jsonObj["amount_water"]["min"] = 0;
    jsonObj["amount_water"]["max"] = 499;
    jsonObj["amount_water"]["default"] = 0;
    ret.insert(EntryEEPROM{187, {1}, "steam_portion_quantity"});                // Dampfportion in Sekunden
    jsonObj["steam_portion"]["min"] = 3;
    jsonObj["steam_portion"]["max"] = 250;
    jsonObj["steam_portion"]["default"] = 10;
    ret.insert(EntryEEPROM{188, {0,1}, "tea_portion_quantity"});                // Tee Portion, ggf. Zeit in 0,1s?!
    jsonObj["steam_portion"]["default"] = 450;

    return ret;
}
