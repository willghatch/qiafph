
#include "globalvars.h"
#include "notes.h"
#include "chordinfo.h"
#include "rulescores.h"



////  Chord Rules

/* -- commented out because currently it can never be nonchord type
int r_no_chord_on_beat(ChordInfo* allchords, int key) {
    int score = 0;
    for (int i = 0; i < nmeasures * mdivision; ++i) {
        if (allchords[i].type == ch_nochord) {
            score += R_NO_CHORD_ON_BEAT;
        }
    }
    return score;
}
*/


int r_non_chord_sdiv(ChordInfo* allchords, int key) {
    int score = 0;
    for (int i = 0; i < nmeasures * mdivision; ++i) {
        score += R_NON_CHORD_SDIV * allchords[i].numNonChordSdivs;
    }
    return score;
}







////  Melody Rules

int r_stepwise_in_key(Note** parts, int key) {
    int score = 0;
    for (int part = 0; part < nparts; ++part) {
        for (int sdiv = 0; sdiv < totalSubdivisions-1; ++sdiv) {
            int cur = parts[part][sdiv].pitch;
            int next = parts[part][sdiv+1].pitch;
            if (cur != next) {
                if (stepwiseInKeyP(cur, next, key)) {
                    score += R_STEPWISE_IN_KEY;
                }
            }
        }
    }
    return score;
}

int r_leading_tone_not_to_tonic(Note** parts, int key) {
    int score = 0;
    for (int part = 0; part < nparts; ++part) {
        for (int sdiv = 0; sdiv < totalSubdivisions-1; ++sdiv) {
            int cur = parts[part][sdiv].pitch;
            int next = parts[part][sdiv+1].pitch;
            if (leadingToneP(cur, key)) {
                if (cur != next && tonicNoteP(next, key)) {
                    score += R_LEADING_TONE_NOT_TO_TONIC;
                }
            }
        }
    }
    return score;
}











int scorePiece(Note** parts, ChordInfo* chords, int** partsIntervals) {
    
    int key = 0;
    int score = 0;
    score += r_non_chord_sdiv(chords, key);
    score += r_stepwise_in_key(parts, key);
    score += r_leading_tone_not_to_tonic(parts, key);
    return score;
}


