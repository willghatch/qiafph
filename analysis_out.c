
#include <stdio.h>
#include "chordinfo.h"
#include "notes.h"
#include "globalvars.h"

void outputAnalysis(char* file, Note** parts, ChordInfo* chords) {
    FILE* f = fopen(file, "w");


    fprintf(f, "Chord progression format: ChordNum [numNonChord]\n");
    for (int i = 0; i < nmeasures * mdivision; ++i) {
        char* typeStr;
        switch(chords[i].type) {
            case ch_maj:
                typeStr = "M";
                break;
            case ch_min:
                typeStr = "m";
                break;
            case ch_dim:
                typeStr = "d";
                break;
            case ch_aug:
                typeStr = "A";
                break;
            case ch_maj_7:
                typeStr = "M7";
                break;
            case ch_dom_7:
                typeStr = "Dom7";
                break;
            case ch_min_7:
                typeStr = "m7";
                break;
            case ch_halfdim_7:
                typeStr = "hdim7";
                break;
            case ch_fulldim_7:
                typeStr = "fdim7";
                break;
            case ch_aug_7:
                typeStr = "aug7";
                break;
            case ch_nochord:
            default:
                typeStr = "NC";
                break;
    
        }
        char* inKeyStr = chords[i].inNormalKeyChordsP ? "" : "@";
        fprintf(f, "%d%s_%s [%d]\n", chords[i].numInPieceKey, inKeyStr, typeStr, chords[i].numNonChordSdivs);
    }


    fclose(f);
}
