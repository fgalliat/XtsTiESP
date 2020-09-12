/**
 * XtsTiESP - mediaplayer.h
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 * Video Media Player API
 * 
 */

// plays a video ".anm" (own format)
bool playANM(char* mediaFile);

// displays a picture ".pic" (own format)
bool playPIC(char* mediaFile);


// plays a full animation (as possible) ".pic" + ".anm" + "_e.pic" (own format)
bool playAnime(char* mediaName);

void textBox(char* text, bool bottom=false);