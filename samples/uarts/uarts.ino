
// HardwareSerial SerialX1(1);

// lib : EspSoftwareSerial
// #include <SoftwareSerial.h>

// RX 1 doens work
#define RX1 10
#define TX1 9
HardwareSerial SerialX1(2);

// that all works
#define TXx  33
#define RXx 32
HardwareSerial SerialX(1); // 5v protected

void setup() {
    Serial.begin(115200);

    SerialX1.begin(115200,SERIAL_8N1,TX1,RX1);

    SerialX.begin(115200,SERIAL_8N1,TXx,RXx);
    // SerialX1.begin(9600);
}


void loop() {
    if (Serial.available() ) {
        int ch = Serial.read();
        Serial.print("#0 ");   Serial.write(ch);
        SerialX1.print("#0 "); SerialX1.write(ch);
        SerialX.print("#0 "); SerialX.write(ch);
    }

    // if (SerialX1.available() ) {
    //     int ch = SerialX1.read();
    //     Serial.print("#1 "); Serial.write(ch);
    //     SerialX1.print("#1 "); SerialX1.write(ch);
    //     SerialX.print("#1 "); SerialX.write(ch);
    // }

    if (SerialX.available() ) {
        int ch = SerialX.read();
        Serial.print("#X "); Serial.write(ch);
        SerialX1.print("#X "); SerialX1.write(ch);
        SerialX.print("#X "); SerialX.write(ch);
    }
}