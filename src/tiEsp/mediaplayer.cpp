/**
 * XtsTiESP - mediaplayer.cpp
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 * Video Media Player API
 * 
 */

#include "hardware.h"

#define ANM_DBUG 0

// #define screen M5.Lcd
#define screen tft

extern int anm_w;
#include "Video_setup.h"
#include "ANM_soft.h"
#include "PIC_soft.h"

// plays a video ".anm" (own format)
bool playANM(char* mediaFile) {

    #if ANM_DBUG
      tft.print("try to play anm: ");
      tft.println(mediaFile);
    #endif

    if ( !sd_exists(mediaFile) ) {
        #if ANM_DBUG
            tft.println("But not here");
        #endif
        return false;
    }


    bool ok = loadAnime(mediaFile);
    if ( !ok ) { return false; }
    playAnime(true);
    closeAnime();

    // FIXME
    return true;
}

// displays a picture ".pic" (own format)
bool playPIC(char* mediaFile) {
    
    #if ANM_DBUG
      tft.print("try to play pic: ");
      tft.println(mediaFile);
    #endif

    if ( !sd_exists(mediaFile) ) {
        #if ANM_DBUG
            tft.println("But not here");
        #endif
        return false;
    }

    bool ok = loadPicture(mediaFile);
    if ( !ok ) { return false; }
    drawPicture();
    closePicture();

    // FIXME
    return true;
}

/**
 * beware : stops if not find the 1st static picture !?
 */
bool playAnime(char* mediaName) {

    #if ANM_DBUG
      tft.print("try to play ainmation: ");
      tft.println(mediaName);
    #endif

    const int fnMax = 64;
    char fullName[ fnMax+1 ];
    memset(fullName, 0x00, fnMax+1 );

    // try to disp. 1st static picture
    sprintf(fullName, "%s.pic", mediaName);
    if ( !playPIC(fullName) ) {
        return false;
    }

    // if found, try to play video
    sprintf(fullName, "%s.anm", mediaName);
    if ( playANM( fullName ) ) {
        // if there is an end-picture : dislpay it
        sprintf(fullName, "%s_e.pic", mediaName);
        playPIC(fullName);
    }

    return true;
}


