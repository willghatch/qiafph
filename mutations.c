
#include "globalvars.h"
#include "notes.h"
#include <stdlib.h>


void changeRandSdivision(Note** parts) {
    int part = rand() % nparts;
    int sdiv = rand() % totalSubdivisions;
    int pitch = (rand() % rangediff[part]) + rangemin[part];
    parts[part][sdiv].pitch = pitch;
    parts[part][sdiv].flags = 0;
}

void changeRandMdivision(Note** parts) {
    int part = rand() % nparts;
    int mdiv = rand() % (nmeasures * mdivision);
    int pitch = (rand() % rangediff[part]) + rangemin[part];
    for(int i = 0; i < sdivPerDiv; ++i) {
        parts[part][mdiv+i].pitch = pitch;
        // TODO - figure out how my tie flag works...
        parts[part][mdiv+i].flags = 0;
    }
}

void changeNRandMdivisions(Note** parts, int n) {
    for (int i = 0; i < n; ++i) {
        changeRandMdivision(parts);
    }
}

void changeNRandSdivisions(Note** parts, int n) {
    for (int i = 0; i < n; ++i) {
        changeRandSdivision(parts);
    }
}






void mutatePiece(Note** parts) {
    int mutation = rand() % 21;
    switch(mutation) {
        case 0:
            changeNRandSdivisions(parts, 20); return;
        case 1:
            changeNRandSdivisions(parts, 1); return;
        case 2:
            changeNRandSdivisions(parts, 2); return;
        case 3:
            changeNRandSdivisions(parts, 3); return;
        case 4:
            changeNRandSdivisions(parts, 4); return;
        case 5:
            changeNRandSdivisions(parts, 5); return;
        case 6:
            changeNRandSdivisions(parts, 6); return;
        case 7:
            changeNRandSdivisions(parts, 7); return;
        case 8:
            changeNRandSdivisions(parts, 8); return;
        case 9:
            changeNRandSdivisions(parts, 9); return;
        case 10:
            changeNRandSdivisions(parts, 10); return;
        case 11:
            changeNRandMdivisions(parts, 1); return;
        case 12:
            changeNRandMdivisions(parts, 2); return;
        case 13:
            changeNRandMdivisions(parts, 3); return;
        case 14:
            changeNRandMdivisions(parts, 4); return;
        case 15:
            changeNRandMdivisions(parts, 5); return;
        case 16:
            changeNRandMdivisions(parts, 6); return;
        case 17:
            changeNRandMdivisions(parts, 7); return;
        case 18:
            changeNRandMdivisions(parts, 8); return;
        case 19:
            changeNRandMdivisions(parts, 9); return;
        case 20:
            changeNRandMdivisions(parts, 10); return;
    }
}

