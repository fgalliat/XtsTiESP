
// Spe mapping for SerialX1 because legacy Serial1 doesn't RX
// 5v protected SerialX

// legacy RX 1 doesn't work
#define RX1 16
#define TX1 9
HardwareSerial SerialX1(1);

// connected to XtsTiLink ProMicro 5v
#define TXx 33
#define RXx 32
HardwareSerial SerialX(2); // 5v protected

void setup() {
    Serial.begin(115200);

    SerialX1.begin(115200,SERIAL_8N1,RX1,TX1);
    SerialX.begin(115200,SERIAL_8N1,RXx,TXx);

    // Serial.println("Hello #0");
    SerialX1.println("Hello #1");
    // SerialX.println("Hello #X");
}


/**
 * \SS for ascii screen dump of Ti92
 * \Ss for binary
 * \SW to wake calc up
 * \SL to launch term Prgm
 */

void loop() {
    if (Serial.available() ) {
        int ch = Serial.read();
        SerialX.write(ch);
        SerialX1.write(ch);
    }

    if (SerialX1.available() ) {
        int ch = SerialX1.read();
        Serial.print("#1 "); Serial.write(ch);
    }

    if (SerialX.available() ) {
        int ch = SerialX.read();
        Serial.write(ch);
    }
}