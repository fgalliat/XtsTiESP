/**
 * XtsTiESP - Video_setup.cpp
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 * Fast SPI ILI9341 routines
 * 
 */

#ifndef SPI_32
 //  #define SPI_32(H,L) ( (H)<<16 | (L) )
 #define SPI_32(H,L) ( ((H)<<8 | (H)>>8) | (((L)<<8 | (L)>>8)<<16 ) )

 #define DC_C digitalWrite(TFT_DC, LOW)
 #define DC_D digitalWrite(TFT_DC, HIGH)
#endif


// extern int anm_w;

static bool locked = false;
static bool inTransaction = false;

#ifdef USE_HSPI_PORT
    static SPIClass _spi = SPIClass(HSPI);
#else // use default VSPI port
    static SPIClass& _spi = SPI;
#endif

inline void _TFT_eSPI_spi_begin(void) {
  if (locked) {locked = false; _spi.beginTransaction(SPISettings(SPI_FREQUENCY, MSBFIRST, TFT_SPI_MODE)); CS_L;}
}

inline void _TFT_eSPI_spi_end(void) {
    if(!inTransaction) {if (!locked) {locked = true; CS_H; _spi.endTransaction();}}
}
// ____________

bool firstDraw = true;

inline void _TFT_moa_placeForPixPush(int x, int y) {
    DC_C;
    
    tft_Write_8(TFT_CASET);
    DC_D;
    tft_Write_32(SPI_32(x, x));
    DC_C;

    tft_Write_8(TFT_PASET);
    DC_D;
    tft_Write_32(SPI_32(y, y));
    DC_C;

    tft_Write_8(TFT_RAMWR);
    DC_D;
}

// ~equiv setWindow
inline void _TFT_moa_placeForLinePush(int x, int y, int zoomX) {

    if ( firstDraw ) {
        // screen.println("=================");
        // screen.print("CASET "); screen.println(TFT_CASET);
        // screen.print("PASET "); screen.println(TFT_PASET);
        // screen.print("RAMWR "); screen.println(TFT_RAMWR);
        // screen.println("=================");
        firstDraw = false;
    }


    DC_C;
    
    tft_Write_8(TFT_CASET);
    DC_D;
    tft_Write_32(SPI_32(0, (anm_w*zoomX)-1));
    DC_C;

    tft_Write_8(TFT_PASET);
    DC_D;
    tft_Write_32(SPI_32(y, y));
    DC_C;

    tft_Write_8(TFT_RAMWR);
    DC_D;
}

inline void _TFT_moa_placeForRemainingLinePush(int x, int y, int zoomX) {
    DC_C;
    
    tft_Write_8(TFT_CASET);
    DC_D;
    tft_Write_32(SPI_32(x*zoomX, ((anm_w*zoomX)-1) ));
    DC_C;

    tft_Write_8(TFT_PASET);
    DC_D;
    tft_Write_32(SPI_32(y, y));
    DC_C;

    tft_Write_8(TFT_RAMWR);
    DC_D;
}

// ex. for raycasters
inline void _TFT_moa_placeRowPush(int x, int y, int y2, bool zoom) {
    DC_C;
    
    tft_Write_8(TFT_CASET);
    DC_D;
    if ( zoom ) {
      tft_Write_32(SPI_32( (x*2) , (x*2)+1 ));  
    } else {
        tft_Write_32(SPI_32(x, x ));
    }
    DC_C;

    tft_Write_8(TFT_PASET);
    DC_D;
    if ( zoom ) {
        tft_Write_32(SPI_32(y*2, y2*2));
    } else {
        tft_Write_32(SPI_32(y, y2));
    }
    DC_C;

    tft_Write_8(TFT_RAMWR);
    DC_D;
}

inline void _TFT_eSPI_pushColor(uint16_t color)
{
 // spi_begin();

  tft_Write_16(color);

 // spi_end();
}

#define MODE_BOOST 1

// uint16_t _anm_fullLine[160];
static uint16_t _anm_fullLine[320]; // for PIC support too
#define ANM_TRANSPARENT 16

static void drawPixLine(int y, bool transp, bool zoom) {
    #if MODE_BOOST
    _TFT_eSPI_spi_begin();
    inTransaction = true;

if ( zoom ) { y *= 2; }
int zoomV = zoom ? 2 : 1;
for(int iter=1; iter <= zoomV; iter++) {

    _TFT_moa_placeForLinePush(0,y, zoomV);
    bool inTransparency = false;
    uint16_t col;
    for(int x=0; x < anm_w; x++) {
        col = _anm_fullLine[x];
        if ( transp && col == ANM_TRANSPARENT ) {
            inTransparency = true;
            continue;
        }
        if ( inTransparency ) {
            // was but no more transparent
            _TFT_moa_placeForRemainingLinePush(x,y, zoomV);
            inTransparency = false;
        }
        _TFT_eSPI_pushColor(col);
        if ( zoom ) { _TFT_eSPI_pushColor(col); }
    }

    y++;
}

    inTransaction = false;
    _TFT_eSPI_spi_end();
    #else
    if ( transp ) {
        M5.Lcd.drawBitmap(0,y,anm_w, 1, _anm_fullLine, (uint16_t)ANM_TRANSPARENT);
        return;
    }
    M5.Lcd.drawBitmap(0,y,anm_w, 1, _anm_fullLine);
    #endif
}


 // 16b color(8b, 8b, 8b)
static uint16_t color565(int r, int g, int b) {
  return (uint16_t) ( (r * 31 / 255 << 11) | (g * 63 / 255 << 5) | (b * 31 / 255) );
}


