/**
 * XtsTiESP - ANM_soft.h
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 */

File anime;
int anm_type = -1;
int anm_fps = -1;
int anm_w = -1;
int anm_h = -1;

// mode 160x120 Video
// only supported format @ this time
#define ANM_HEADER_160 160

// same line
#define ANM_FLAG_LINE_SAME 0
// some diffs in line
#define ANM_FLAG_LINE_DIFF 1
// line totaly replaced
#define ANM_FLAG_LINE_REPL 2
// whole same frame
#define ANM_FLAG_FRAME_SAME 3

bool loadAnime(char* filename) {
    yield();
    if ( ! sd_exists(filename) ) {
        yield();
        screen.println("(E) Anime not exists");
        // file not exists
        return false;
    }

    anime = sd_openFile(filename);
    if (!anime) {
        // file can't open
        screen.println("(E) Anime can't open");
        return false;
    }

    if ( anime.size() < 4 ) {
        // file too short
        screen.println("(E) Anime too short");
        return false;
    }

    if ( (char)anime.read() != 'A' ) {
        return false;
    }
    if ( (char)anime.read() != 'N' ) {
        return false;
    }
    if ( (char)anime.read() != 'M' ) {
        return false;
    }

    if ( anime.read() != ANM_HEADER_160 ) {
        // not supported format
        screen.println("(E) Anime wrong format");
        return false;
    }
    anm_type = ANM_HEADER_160;

    anm_w = anime.read();
    anm_h = anime.read();
    anm_fps = anime.read();

    return true;
}

void closeAnime() {
    if (!anime) {
        return;
    }
    anime.close();
}

// ____________

void _drawAnimeLine(int y, int flag, bool zoom) {
    uint16_t color;
    uint8_t seq[3];
    if ( flag == ANM_FLAG_LINE_REPL ) {
        // for(int x=0; x < anm_w; x++) {
        //     anime.read(seq, 2);
        //     color = (seq[0]<<8)+seq[1];
        //     _anm_fullLine[x] = color;
        // }

        uint8_t _anm_fullLine8[ 2*anm_w ];
        anime.read( _anm_fullLine8, 2*anm_w);
        int addr;
        for(int x=0; x < anm_w; x++) {
            addr = x << 1;
            color = (_anm_fullLine8[addr]<<8)+_anm_fullLine8[addr+1];
            _anm_fullLine[x] = color;
        }


        drawPixLine(y, false, zoom);
    } else {
        for(int x=0; x < anm_w; x++) {
            _anm_fullLine[x] = ANM_TRANSPARENT;
        }

        int diffs = anime.read();
        int x;
        for(int cpt=0; cpt < diffs; cpt++) {
            anime.read(seq, 3);
            x = seq[0];
            color = (seq[1]<<8)+seq[2];
            _anm_fullLine[x] = color;
        }
        drawPixLine(y, true, zoom);
    }
}

void _drawAnimeFrame(bool zoom) {
    int flag;
    for(int y = 0; y < anm_h; y++) {
        flag = anime.read();
        if ( flag == ANM_FLAG_FRAME_SAME ) { break; }
        if ( flag == ANM_FLAG_LINE_SAME ) { continue; }
        _drawAnimeLine(y, flag, zoom);
    }
}

void playAnime(bool zoom = false) {
    if ( anm_type != ANM_HEADER_160 ) {
        return;
    }

    int period = 1000 / anm_fps;
    // period /= 10;

    long t0,t1;
    while( anime.available() ) {
        t0 = millis();
        _drawAnimeFrame(zoom);
        t1 = millis();
        if ( t1 - t0 < period ) {
            delay( period - ( t1 - t0 ) );
        }
    }

}
