
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

int r_chord_in_normal_key_chords(ChordInfo* allchords, int key) {
    // Umm... assume it's major, I guess
    int score = 0;
    for (int i = 0; i < nmeasures * mdivision; ++i) {
        if (allchords[i].numInPieceKey_M != 0) {
            score += R_CHORD_IN_NORMAL_KEY_CHORDS;
        }
    }
    return score;
}

int r_full_chord(ChordInfo* allchords, int key) {
    int score = 0;
    for (int i = 0; i < nmeasures * mdivision; ++i) {
        if (allchords[i].fullChordP) {
            score += R_FULL_CHORD;
        }
    }
    return score;
}

int r_start_chord(ChordInfo* allchords, int key) {
    int sc = allchords[0].numInPieceKey_M;
    switch (sc) {
        case 1:
            return R_START_CHORD__I;
        case 5:
            return R_START_CHORD__V;
        default:
            return 0;
    }
}

int r_end_cadence(ChordInfo* allchords, int key) {
    // TODO - write functions to get cadence info and make this better
    int ecIndex = nmeasures * mdivision - 1;
    if (allchords[ecIndex].numInPieceKey_M == 1) {
        ChordInfo ntl = allchords[ecIndex-1];
        if (ntl.numInPieceKey_M == 5) {
            return R_END_CADENCE__AUTHENTIC;
        } else if (ntl.numInPieceKey_M == 4) {
            return R_END_CADENCE__PLAGAL;
        }
    }
    return 0;
}

int r_chord_progressions(ChordInfo* allchords, int key) {
    int score = 0;
    for (int i = 1; i < nmeasures * mdivision; ++i) {
        ChordInfo a = allchords[i-1];
        ChordInfo b = allchords[i];
        switch(a.numInPieceKey_M) {
            case 1:
                switch(b.numInPieceKey_M) {
                    case 1:
                        break;
                    case 2:
                        score += R_CHORD_PROG__I_ii;
                        break;
                    case 3:
                        break;
                    case 4:
                        score += R_CHORD_PROG__I_IV;
                        break;
                    case 5:
                        score += R_CHORD_PROG__I_V;
                        break;
                    case 6:
                        score += R_CHORD_PROG__I_vi;
                        break;
                    case 7:
                        break;
                }
                break;
            case 2:
                switch(b.numInPieceKey_M) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        score += R_CHORD_PROG__ii_V;
                        break;
                    case 6:
                        break;
                    case 7:
                        score += R_CHORD_PROG__ii_viid;
                        break;
                }
                break;
            case 3:
                switch(b.numInPieceKey_M) {
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        score += R_CHORD_PROG__iii_IV;
                        break;
                    case 5:
                        break;
                    case 6:
                        score += R_CHORD_PROG__iii_vi;
                        break;
                    case 7:
                        break;
                }
                break;
            case 4:
                switch(b.numInPieceKey_M) {
                    case 1:
                        score += R_CHORD_PROG__IV_I;
                        break;
                    case 2:
                        score += R_CHORD_PROG__IV_ii;
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        score += R_CHORD_PROG__IV_V;
                        break;
                    case 6:
                        break;
                    case 7:
                        score += R_CHORD_PROG__IV_viid;
                        break;
                }
                break;
            case 5:
                switch(b.numInPieceKey_M) {
                    case 1:
                        score += R_CHORD_PROG__V_I;
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        break;
                    case 6:
                        score += R_CHORD_PROG__V_vi;
                        break;
                    case 7:
                        break;
                }
                break;
            case 6:
                switch(b.numInPieceKey_M) {
                    case 1:
                        break;
                    case 2:
                        score += R_CHORD_PROG__vi_ii;
                        break;
                    case 3:
                        break;
                    case 4:
                        score += R_CHORD_PROG__vi_IV;
                        break;
                    case 5:
                        score += R_CHORD_PROG__vi_V;
                        break;
                    case 6:
                        break;
                    case 7:
                        break;
                }
                break;
            case 7:
                switch(b.numInPieceKey_M) {
                    case 1:
                        score += R_CHORD_PROG__viid_I;
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    case 5:
                        score += R_CHORD_PROG__viid_V;
                        break;
                    case 6:
                        break;
                    case 7:
                        break;
                }
                break;
        }
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

int r_note_not_in_key(Note** parts, int key) {
    int score = 0;
    for (int part = 0; part < nparts; ++part) {
        for (int sdiv = 0; sdiv < totalSubdivisions-1; ++sdiv) {
            Note note = parts[part][sdiv];
            if (!noteInKeyP(note, key)) {
                score += R_NOTE_NOT_IN_KEY;
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
    score += r_note_not_in_key(parts, key);
    score += r_chord_in_normal_key_chords(chords, key);
    score += r_full_chord(chords, key);
    score += r_start_chord(chords, key);
    score += r_end_cadence(chords, key);
    return score;
}


