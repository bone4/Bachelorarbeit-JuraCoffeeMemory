#include "RAM_Status.hpp"

using namespace std;

RAM_Status::RAM_Status()
{
    //ctor
}

RAM_Status::~RAM_Status()
{
    //dtor
}

void RAM_Status::scan() {
    SerialConnection& s = SerialConnection::getInstance();
    s.connect();



    // 0x00
    raw.push_back("00");

    // 0x01 - 0x10
    for (int i=1; i<=16; i++) { known_bytes.insert(i); }
    s.send("RR:01");
    string resp1 = s.receive();
    string data1 = resp1.substr(3); // cut e.g. "rr:" off
    data1 = removeSpecialCharFromString(data1); // remove chars e.g. '\r'
    raw.push_back(data1);

    // 0x11 - 0x15
    for (int i=17; i<=21; i++) { raw.push_back("00"); }

    // 0x16 - 0x25
    for (int i=22; i<=37; i++) { known_bytes.insert(i); }
    s.send("RR:16");
    string resp2 = s.receive();
    string data2 = resp2.substr(3); // cut e.g. "rr:" off
    data2 = removeSpecialCharFromString(data2); // remove chars e.g. '\r'
    raw.push_back(data2);

    // 0x26 - 0x43
    for (int i=38; i<=67; i++) { raw.push_back("00"); }

    // 0x44 - 0x53
    for (int i=68; i<=83; i++) { known_bytes.insert(i); }
    s.send("RR:44");
    string resp3 = s.receive();
    string data3 = resp3.substr(3); // cut e.g. "rr:" off
    data3 = removeSpecialCharFromString(data3); // remove chars e.g. '\r'
    raw.push_back(data3);

    // 0x54 - 0x61
    for (int i=84; i<=97; i++) { raw.push_back("00"); }

    // 0x62 - 0x71
    //s.send("RR:62");
    //string resp = s.receive();
    //string data = resp.substr(3); // cut e.g. "rr:" off
    //data = removeSpecialCharFromString(data); // remove chars e.g. '\r'
    //raw.push_back(data);
    for (int i=98; i<=113; i++) { raw.push_back("00"); }

    // 0x72 - 0x79
    for (int i=114; i<=121; i++) { raw.push_back("00"); }

    // 0x7A - 0x89
    for (int i=122; i<=137; i++) { known_bytes.insert(i); }
    s.send("RR:80");
    string resp4 = s.receive();
    string data4 = resp4.substr(3); // cut e.g. "rr:" off
    data4 = removeSpecialCharFromString(data4); // remove chars e.g. '\r'
    raw.push_back(data4);

    // 0x8A - 0xEF
    for (int i=138; i<=239; i++) { raw.push_back("00"); }

    // 0xF0 - 0xFF
    //s.send("RR:F0");
    //string resp = s.receive();
    //string data = resp.substr(3); // cut e.g. "rr:" off
    //data = removeSpecialCharFromString(data); // remove chars e.g. '\r'
    //raw.push_back(data);
    for (int i=240; i<=255; i++) { raw.push_back("00"); }



    s.disconnect();

    for (auto &row : raw) {
        hexString2int(row, bytes);
    }
}

string RAM_Status::get_RAM_Status() {
    set<EntryRAM> table = getEntriesRAM();

    // 1. check if raw is not empty -> call scan()
    scan();
//    for (auto i : known_bytes) {
//        cout << to_string(i) << " ";
//    }
//    cout << endl;
//    printRawVector();

    // 2. check if "std::vector<int> bytes" contains of exactly 256 Bytes
    if (bytes.size() != 256) {
        cerr << "Information of 256 bytes RAM are needed, but " << to_string(bytes.size()) << " are given." << endl;
        exit(6);
    }

    for (auto ent : table) {
        // 3. check if getEntriesRAM.bytes are in known_bytes (safety)
        if (known_bytes.find(ent.byte) == known_bytes.end()) {
            cerr << "Byte " << to_string(ent.byte) << " in RAM_Status::getEntriesRAM() is not known from RAM_Status::scan()!" << endl;
            exit(7);
        }

        // 4. loop over getEntriesRAM, extract the bits and build the json object
        switch (ent.bits.size()) {
            case 0: // whole byte
                jsonObj[ent.label] = bytes[ent.byte];
                break;
            case 1: // bool
                jsonObj[ent.label] = getBit(bytes[ent.byte], *(ent.bits.begin()));
                break;
            default: // integer of the corresponding bits
                int z = 0;
                for (auto i : ent.bits) {
                    if (getBit(bytes[ent.byte], i)) { // if bit is set
                        z += pow(2, i); // increase the integer by the representing number of the actual bit
                    }
                }
                jsonObj[ent.label] = z;
                break;
        }
    }

    // 5. return json object as short string
    return fast.write(jsonObj);
}
void RAM_Status::pretty_print_json() {
    cout << jsonObj << endl; // Json::StyledStreamWriter for operator<<
}

set<EntryRAM> RAM_Status::getEntriesRAM() {
    set<EntryRAM> ret;

    ret.insert(EntryRAM{  3, {7}, "brew_step1_grinder_on"});     // Zubereitung Schritt 1, Mahlwerk ist an
    ret.insert(EntryRAM{  3, {2}, "on"});                        // Maschine ist an
    ret.insert(EntryRAM{  4, {0}, "fill_powder"});               // Pulver füllen
    ret.insert(EntryRAM{ 10, {6}, "coffee_is_made"});            // Kaffee wird zubereitet (Schritt 1 und 2)
    ret.insert(EntryRAM{ 10, {2,1,0}, "brew_coffee_type"});      // Art des zubereiteten Kaffees (1=1kleiner Kaffee, 2=2 kleine Kaffees, 3=1großer Kaffee, 4=2große Kaffees, 5=spezial Kaffee)
    ret.insert(EntryRAM{ 13, {3,2}, "rinsing_before_switching_off"}); // eine Zubereitung ist erfolgt, es wird vor dem Ausschalten automatisch gespült
    ret.insert(EntryRAM{ 14, {7}, "fill_beans"});                // Bohnen füllen (nach eienr Zubereitung mit ungenügend Bohnen)
    ret.insert(EntryRAM{ 14, {6}, "fill_water"});                // Wasser füllen (Wasser leer oder Wassertank entnommen)
    ret.insert(EntryRAM{ 14, {5}, "empty_grounds"});             // Trester muss geleert werden
    ret.insert(EntryRAM{ 14, {4}, "empty_drip_tray"});           // Schale leeren
    ret.insert(EntryRAM{ 14, {2}, "tray_missing"});              // Schale fehlt
    ret.insert(EntryRAM{ 15, {6}, "tap_closed"});                // Hahn geschlossen
    ret.insert(EntryRAM{ 15, {5}, "tea_portion"});               // Teeportion (in der Zubereitung)
    ret.insert(EntryRAM{ 15, {2,1}, "cup_illumination"});        // Tassenbeleuchtung
    ret.insert(EntryRAM{ 16, {7}, "clean_machine"});             // Gerät reinigen
    ret.insert(EntryRAM{ 16, {5}, "replacing_filter"});          // Filter wechseln (50 Liter erreicht)
    ret.insert(EntryRAM{ 23, {3}, "brew_step2_pumping_coffee"}); // Zubereitung Schritt 2, Zubereitung eines Kaffees, nach dem Bohnen mahlen
    ret.insert(EntryRAM{ 23, {1}, "rinsing_unit"});              // Maschine spült
    ret.insert(EntryRAM{ 73, {0}, "steam_portion"});             // Dampfbezug / Wasserdampfportion
    ret.insert(EntryRAM{128, {1}, "empty_grounds_reason_only_0x34_>_0x03BF"}); // Trester leeren, ABER NUR durch 0x34 >= 0x03C0

    return ret;
}
