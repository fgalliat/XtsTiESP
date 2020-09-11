/**
 * XtsTiESP - PIC_soft.h
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 */

#define PIC_HEADER_320 2

bool loadPicture(char* filename) {
    yield();
    if ( ! sd_exists(filename) ) {
        yield();
        screen.println("(E) Picture not exists");
        // file not exists
        return false;
    }

    anime = sd_openFile(filename);
    if (!anime) {
        // file can't open
        screen.println("(E) Picture can't open");
        return false;
    }

    if ( anime.size() < 4 ) {
        // file too short
        screen.println("(E) Picture too short");
        return false;
    }

    if ( (char)anime.read() != 'P' ) {
        return false;
    }
    if ( (char)anime.read() != 'I' ) {
        return false;
    }
    if ( (char)anime.read() != 'C' ) {
        return false;
    }

    if ( anime.read() != (int)'3' ) { // 320 like format
        // not supported format
        screen.println("(E) Picture wrong format");
        return false;
    }
    anm_type = PIC_HEADER_320;

    anm_w = (anime.read() << 8) + anime.read();
    anm_h = (anime.read() << 8) + anime.read();
    // anm_fps = anime.read();

    return true;
}

void closePicture() {
    if (!anime) {
        return;
    }
    anime.close();
}

void _drawStaticFrame() {
    int flag = ANM_FLAG_LINE_REPL;
    for(int y = 0; y < anm_h; y++) {
        _drawAnimeLine(y, flag, false);
    }
}

void drawPicture() {
 _drawStaticFrame();   
}