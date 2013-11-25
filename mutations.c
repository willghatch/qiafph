
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






void mutatePiece(Note** parts) {
    changeRandMdivision(parts);
}

