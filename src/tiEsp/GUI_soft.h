/**
 * XtsTiESP - GUI_soft.h
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 */

void poll() {
    // noting for now
}

#define M5 0

// #define screen M5.Lcd
#define screen tft

// #define pollInput M5.update
#define pollInput poll

#define KEY_NONE       0x00
#define KEY_DUP        0x01
#define KEY_DDOWN      0x02
#define KEY_DLEFT      0x03
#define KEY_DRIGHT     0x04
#define KEY_BA         0x05
#define KEY_BB         0x06
#define KEY_BSELELECT  0x07
#define KEY_BSTART     0x08

int getKeypad() {
//   // read FaceKB GameBoyPad
//   if(digitalRead(FACEKB_INT) == LOW) {
//     Wire.requestFrom(FACEKB_I2C_ADDR, 1);  // request 1 byte from keyboard
//     while (Wire.available()) { 
//       uint8_t key_val = Wire.read();                  // receive a byte as character
//       if(key_val != 0) {
//           // happens w/ gameboy faces
//           // (..) when release the key (act as onKeyUp)
//           if ( key_val == 0xFF ) { ; } 
//           else {
//             // Serial.printf("0x%02X ",key_val);
//             // tft.printf("0x%02X ",key_val);

//             if ( key_val == 0xFE ) { return KEY_DUP; }
//             if ( key_val == 0xFD ) { return KEY_DDOWN; }
//             if ( key_val == 0xFB ) { return KEY_DLEFT; }
//             if ( key_val == 0xF7 ) { return KEY_DRIGHT; }

//             if ( key_val == 0xEF ) { return KEY_BA; }
//             if ( key_val == 0xDF ) { return KEY_BB; }

//             if ( key_val == 0xBF ) { return KEY_BSELELECT; }
//             if ( key_val == 0x7F ) { return KEY_BSTART; }

//           }
//       }
//     }
//   }
  return KEY_NONE;
}



struct Point
{
    int x;
    int y;
};

Point* txtPoint = NULL;

Point* getTextSize(char* str) {
    if ( txtPoint == NULL ) { txtPoint = (Point*)malloc( sizeof(Point) ); }
    txtPoint->x = 0;
    txtPoint->y = 1;

    int tlen = strlen(str);
    int x=0;
    for(int i=0; i < tlen; i++) {
        char c = str[i];
        if ( c == '\n' ) {
            txtPoint->y++;
            x=0;
            continue;
        }
        x++;
        if ( x > txtPoint->x ) { txtPoint->x = x; }
    } 
    if ( x > txtPoint->x ) { txtPoint->x = x; }

    return txtPoint;
}

int ftWidth = 6;
int ftHeight = 8;

Point* getMenuSize(char* text, char** items, int nbItems) {
    Point* p = getTextSize(text);

    p->y++;

    for(int i=0; i < nbItems; i++) {
        int tlen = strlen( items[i] );
        if ( tlen > p->x ) { p->x = tlen; }
        p->y++;
    }

    return p;
}

int scWidth = 320;
int scHeight = 240;

int menu(char* text, char** items, int nbItems) {
    Point* p = getMenuSize(text, items, nbItems);

    int w = p->x*ftWidth;
    int h = p->y*ftHeight;

    int x = ( scWidth - w ) / 2;
    int y = ( scHeight - h - 20 );

    screen.fillRect( x-3, y-3, w+6, h+6, TFT_BLUE );
    screen.drawRect( x-3, y-3, w+6, h+6, TFT_WHITE );

    int xx = x; int yy = y;
    screen.setCursor( xx,yy );
    // screen.println( text );
    int tlen = strlen( text );
    for(int i=0; i < tlen; i++) {
        char c = text[i];
        if ( c == '\n' ) {
            xx = x;
            yy += ftHeight;
            continue;
        }
        screen.setCursor( xx,yy );
        screen.write(c);
        xx += ftWidth;
    }
    yy+=ftHeight;

    yy+=ftHeight;
    int YY = yy;

    int selected = 0;
    do {

        yy = YY;

        xx = x;
        for(int i=0; i < nbItems; i++) {
            screen.setCursor( xx,yy );
            if ( i == selected ) { screen.print( "> " ); }
            else { screen.print( "  " ); }
            screen.print( items[i] );
            yy+=ftHeight;
        }

        while(true) {
            pollInput();
            int key = getKeypad();
#if M5
            if ( M5.BtnB.isPressed() || key == KEY_BA  || key == KEY_BB ) {
                while (M5.BtnB.isPressed()) { M5.update();}
                return selected;
            }

            if ( M5.BtnA.isPressed() || key == KEY_DLEFT  || key == KEY_DUP ) {
                if ( selected == 0 ) { selected = nbItems-1; }
                else selected--;

                while (M5.BtnA.isPressed()) { M5.update();}
                break;
            }

            if ( M5.BtnC.isPressed()|| key == KEY_DRIGHT  || key == KEY_DDOWN ) {
                if ( selected == nbItems-1 ) { selected = 0; }
                else selected++;
                while (M5.BtnC.isPressed()) { M5.update();}
                break;
            }
#endif
            delay(100);
        }

    } while(true);

    
    return -1;
}

// ==========================================

void textBox(char* text, bool bottom=false) {
    Point* p = getTextSize(text);

    int w = p->x*ftWidth;
    int h = p->y*ftHeight;

    int x = ( scWidth - w ) / 2;
    int y = 20;
    if ( bottom ) {
        y = ( scHeight - h - 20 );
    }

    screen.fillRect( x-3, y-3, w+6, h+6, TFT_BLUE );
    screen.drawRect( x-3, y-3, w+6, h+6, TFT_WHITE );

    int xx = x; int yy = y;
    screen.setCursor( xx,yy );
    // screen.println( text );
    int tlen = strlen( text );
    for(int i=0; i < tlen; i++) {
        char c = text[i];
        if ( c == '\n' ) {
            xx = x;
            yy += ftHeight;
            continue;
        }
        screen.setCursor( xx,yy );
        screen.write(c);
        xx += ftWidth;
    }
    yy+=ftHeight;
}

// ==========================================

#define MAX_POINTS 15

Point** pts = NULL;
int ptsNb = 0;
int ptsSelected = 0;

void log(char* str) { Serial.println(str); }

void clearPoints() {
    if ( pts == NULL ) {
        pts = (Point**)malloc( MAX_POINTS * sizeof(Point) );
        for(int i=0; i < MAX_POINTS; i++) {
            pts[i] = (Point*)malloc( 1 * sizeof(Point) );
        }
    }
    ptsNb = 0;
    ptsSelected = 0;
}

void submitPoint(int x, int y) {
    if ( pts == NULL ) {
       clearPoints(); 
    }

    pts[ptsNb]->x = x;
    pts[ptsNb]->y = y;
    ptsNb++;
}

void drawPoint(int num) {
    int color = ptsSelected == num  ? TFT_RED : TFT_BLUE;

    int x = pts[ num ]->x;
    int y = pts[ num ]->y;

    screen.fillRect( x-2, y-2, 4, 4, color );
    screen.drawCircle( x-3, y-3, 6, color );
}

void drawPoints() {
    for(int i=0; i < ptsNb; i++) { drawPoint(i); }
}

int waitSelectPoint() {
    drawPoints();

while(true) {

    while(true) {
        pollInput();
        int key = getKeypad();
#if M5
        if ( M5.BtnB.isPressed() || key == KEY_BA  || key == KEY_BB ) {
            while (M5.BtnB.isPressed()) { M5.update();}
            return ptsSelected;
        }

        if ( M5.BtnA.isPressed() || key == KEY_DLEFT  || key == KEY_DUP  ) {
            if ( ptsSelected == 0 ) { ptsSelected = ptsNb-1; }
            else ptsSelected--;

            while (M5.BtnA.isPressed()) { M5.update();}
            break;
        }

        if ( M5.BtnC.isPressed() || key == KEY_DRIGHT  || key == KEY_DDOWN ) {
            if ( ptsSelected == ptsNb-1 ) { ptsSelected = 0; }
            else ptsSelected++;
            while (M5.BtnC.isPressed()) { M5.update();}
            break;
        }
#endif
        delay(100);
    }
    drawPoints();
}

    return -1;
}


void error(char* str) {
    screen.clear();
    screen.setCursor(0,0);
    screen.println("===========");
    screen.println( str );
    screen.println("===========");

    while(true) {
        delay(400);        
    }
}















