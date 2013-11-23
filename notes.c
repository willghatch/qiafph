// notes.c

#include "notes.h"

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


