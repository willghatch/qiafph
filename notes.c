// notes.c

#include "notes.h"
#include "globalvars.h"
#include "chordinfo.h"

char getNoteName(Note n) {
    int p = n.pitch;
    p = p % 12;
    switch(p) {
        case CNOTE:
            return 'C';
        case DNOTE:
        case DbNOTE:
            return 'D';
        case ENOTE:
        case EbNOTE:
            return 'E';
        case FNOTE:
            return 'F';
        case GNOTE:
        case GbNOTE:
            return 'G';
        case ANOTE:
        case AbNOTE:
            return 'A';
        case BNOTE:
        case BbNOTE:
            return 'B';
    }
}

char getNoteName_lowercase(Note n) {
    char c = getNoteName(n);
    return c + 32;
}

int flat_p(Note n) {
    int p = n.pitch % 12;
    switch(p) {
        case AbNOTE:
        case BbNOTE:
        case DbNOTE:
        case EbNOTE:
        case GbNOTE:
            return 1;
        default:
            return 0;
    }
}

int getNoteOctave(Note n) {
    return n.pitch / 12;
}




// Scoring type predicates...

int leadingToneP(int pitch, int key) {
    int lt = (key + 11) % 12;
    pitch %= 12;
    return pitch == lt;
}

int tonicNoteP(int pitch, int key) {
    pitch %= 12;
    return pitch == key;
}

int noteInKeyP(Note n, int key) {
    int p = n.pitch;
    p = (p + key) % 12;
    switch(p) {
        case KP_DO:
        case KP_RE:
        case KP_MI:
        case KP_FA:
        case KP_SOL:
        case KP_LA:
        case KP_TI:
            return 1;
        default:
            return 0;
    }
}

int stepwiseInKeyP(int p1, int p2, int key) {
    int p1KeyOffset = (p1 + key) % 12;
    //if( this is a major key) {
    if (1) {
        if (p1 > p2) { // ascending
            switch(p1KeyOffset) {
                case KP_DO:
                case KP_RE:
                case KP_MI:
                case KP_SOL:
                case KP_LA:
                    return p1 - p2 == I_M2;

                case KP_FA:
                case KP_TI:
                    return p1 - p2 == I_m2;
            }
        } else { // descending
            switch(p1KeyOffset) {
                case KP_RE:
                case KP_MI:
                case KP_SOL:
                case KP_LA:
                case KP_TI:
                    return p2 - p1 == I_M2;

                case KP_DO:
                case KP_FA:
                    return p2 - p1 == I_m2;
            }
        }
    }
}


