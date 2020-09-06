#define LED 27

#define BUZZER 12
#define BUZ_channel 0
#define BUZ_resolution 8


// labeled RXD but dones't receives serial datas on my model
#define BTN 10

void setup() {
    Serial.begin(115200);


    pinMode( LED, OUTPUT );
    digitalWrite( LED, LOW );

    pinMode( BUZZER, OUTPUT );
    digitalWrite( BUZZER, LOW );
    int freq = 2000;
    ledcSetup(BUZ_channel, freq, BUZ_resolution);
    ledcAttachPin(BUZZER, BUZ_channel);

    pinMode( BTN, INPUT_PULLUP );
}

// ======== Buzzer ================
void _tone(int freq) {
    int vol = 125;
    vol = 200;
    ledcWrite(BUZ_channel, vol); // volume
    ledcWriteTone(BUZ_channel, freq); // freq
}

void _noTone() {
    ledcWrite(BUZ_channel, 0); // volume  
}

void beep(int freq, int duration) {
    _tone(freq);
    delay(duration);
    _noTone();
}
// ======== Buzzer ================

void loop() {
    digitalWrite( LED, HIGH );
    beep(440, 200);

    if ( digitalRead( BTN ) == LOW ) {
        Serial.println("Btn pressed");
    } else {
        //..
    }

    delay(500);
    digitalWrite( LED, LOW );
    delay(200);
}