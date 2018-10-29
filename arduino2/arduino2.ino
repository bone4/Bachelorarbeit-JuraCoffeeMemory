#include <SoftwareSerial.h>

SoftwareSerial coffemaker_serial(12, 13);  /* RX TX */

static const byte intra_group_delay = 1; /* delay between individual bytes of group */
static const byte inter_group_delay = 2; /* delay between groups */

/*
 * Contains four-byte-group representing one byte of plain data
 */
class CoffemakerByte {
private:
    byte data[4];

public:
    CoffemakerByte() : data{0xFF, 0xFF, 0xFF, 0xFF} {}
    byte operator[](byte index) const { return data[index]; }
    byte& operator[](byte index) { return data[index]; }
};

void setup() {
    Serial.begin(9600);             /* Serial interface towards master */
    coffemaker_serial.begin(9600);  /* Serial interface towards jura coffee machine */
}

void loop() {
    String txt;
    /* Forward bytes from master to coffee machine after encoding */
    while(Serial.available()) {
        txt = Serial.readString();
        if (txt.startsWith("Y") == true) {        /* RAM */
          transmit_string_to_coffemaker("RR:00");
          transmit_byte_to_coffemaker(0x0D);
          transmit_byte_to_coffemaker(0x0A);
          delay(1100);
          while(coffemaker_serial.available()){
              Serial.print(char(receive_byte_from_coffemaker()));
          }
          for (int i = 0; i < 255; i+=16) {
            transmit_string_to_coffemaker("RR:");
            transmit_string_to_coffemaker(String(i));
            transmit_byte_to_coffemaker(0x0D);
            transmit_byte_to_coffemaker(0x0A);
            delay(500);
            while(coffemaker_serial.available()){
              Serial.print(char(receive_byte_from_coffemaker()));
            }
            Serial.println();
          }
        } else if (txt.startsWith("Z") == true) { /* EEPROM */
          transmit_string_to_coffemaker("RT:00");
          transmit_byte_to_coffemaker(0x0D);
          transmit_byte_to_coffemaker(0x0A);
        } else {
          transmit_string_to_coffemaker(txt);
        }
    }

    /* Forward bytes from coffee machine to master after decoding */
    while(coffemaker_serial.available()){
        Serial.print(char(receive_byte_from_coffemaker()));
    }
}

/*
 * Convert four UART bytes to a single data byte
 *
 * in:
 * byte: 3        2        1        0
 * bit:  --7--6-- --5--4-- --3--2-- --1--0--
 *
 * out:
 * byte: 0
 * bit:  76543210
 */
byte coffemaker_to_plain(const CoffemakerByte in){
    byte out = 0x00;

    for (byte i = 0; i < 4; i++) {
        bitWrite(out, 2 * i, bitRead(in[i], 2));
        bitWrite(out, 2 * i + 1, bitRead(in[i], 5));
    }

    return out;
}

/*
 * Convert data byte to four UART bytes
 *
 * in:
 * byte: 0
 * bit:  76543210
 *
 * out:
 * byte: 3        2        1        0
 * bit:  --7--6-- --5--4-- --3--2-- --1--0--
 */
CoffemakerByte plain_to_coffemaker(byte in) {
    CoffemakerByte out;

    for (byte i = 0; i < 4; i++) {
        bitWrite(out[i], 2, bitRead(in, 2 * i));
        bitWrite(out[i], 5, bitRead(in, 2 * i + 1));
    }

    return out;
}

/*
 * Transmit representation of a data byte to the coffee maker
 */
void transmit_byte_to_coffemaker(byte plain) {
    CoffemakerByte converted = plain_to_coffemaker(plain);

    for (byte i = 0; i < 4; i++) {
        delay(intra_group_delay);
        coffemaker_serial.write(converted[i]);
    }
    delay(inter_group_delay);

    return;
}

void transmit_string_to_coffemaker(String txt) {
  for (int i = 0; i < txt.length(); i++) {
    transmit_byte_to_coffemaker((byte)txt.charAt(i));
  }
}

/*
 * Receive one data byte from the coffe maker
 */
byte receive_byte_from_coffemaker() {
    CoffemakerByte raw;

    for (byte i = 0; i < 4; i++) {
        delay(intra_group_delay);
        raw[i] = coffemaker_serial.read();
    }

    return coffemaker_to_plain(raw);
}
