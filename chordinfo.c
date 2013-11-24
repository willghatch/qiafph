//chordinfo.c

#include "chordinfo.h"
#include "globalvars.h"
#include "notes.h"
#include <string.h>
#include <stdlib.h>

ChordInfo* allocChordArray() {
    ChordInfo* ptr = malloc(sizeof(ChordInfo)*nmeasures*(mdivision/sdivPerDiv));
}

int minorChordP(ChordType t) {
    return t == ch_min || t == ch_min_7;
}
int diminishedChordP(ChordType t) {
    return t == ch_dim || t == ch_halfdim_7 || ch_fulldim_7;
}

int countNonChordTones(int* intervalCount, int sdivs, ChordType ct, int* fullChordP) {
// intervalCount is a 12 int array of the number of each interval type
// sdivs is the total number of subdivisions/intervals
// also sets the variable in fullChordP to true or false
    // start with all of them, and subtract the valid ones
    int ncc = sdivs;
    *fullChordP = 1;
    ncc -= intervalCount[I_UN];
    if (intervalCount[I_UN] == 0)
        {*fullChordP = 0;}
    if (minorChordP(ct) || diminishedChordP(ct)) {
        ncc -= intervalCount[I_m3];
        if(intervalCount[I_m3] == 0)
            {*fullChordP = 0;}
    } else {
        ncc -= intervalCount[I_M3];
        if(intervalCount[I_M3] == 0)
            {*fullChordP = 0;}
    }
    if (diminishedChordP(ct)) {
        ncc -= intervalCount[I_d5];
        if(intervalCount[I_d5] == 0)
            {*fullChordP = 0;}
    } else {
        ncc -= intervalCount[I_P5];
        if(intervalCount[I_P5] == 0)
            {*fullChordP = 0;}
    }

    if (ct == ch_fulldim_7)
        {ncc -= intervalCount[I_M6];}
    if (ct == ch_halfdim_7 || ct == ch_min_7 || ct == ch_dom_7)
        {ncc -= intervalCount[I_m7];}
    if (ct == ch_maj_7)
        {ncc -= intervalCount[I_M7];}
    return ncc;
}

void getChordInfo_most(int* sdivPitches, int npitches, ChordInfo* singleChord) {
    // analyzes the n pitches in sdivPitches and modifies the single chordinfo (most of it, at least)

    for(int i = 0; i < npitches; ++i) {
        if (sdivPitches[i] > 11) {
            sdivPitches[i] %= 12;
        }
    }

    int bestFitRoot = 0;
    int bestFitScore = 0;
    int intervals[12] = {0}; //best fit ones
    for(int note = 0; note < 12; ++note) {
        int score = 0;
        int temp_intervals[12] = {0};
        // Check each note to see if it could be the root
        for(int sd = 0; sd < npitches; ++sd) {
            int interval = (sdivPitches[sd] + 12 - note) % 12;
            temp_intervals[interval]++;
            switch(interval) {
                case I_UN:
                    score += CH_SC_UNISON; break;
                case I_m3:
                    score += CH_SC_m3; break;
                case I_M3:
                    score += CH_SC_M3; break;
                case I_d5:
                    score += CH_SC_d5; break;
                case I_P5:
                    score += CH_SC_p5; break;
                case I_M6:
                    score += CH_SC_d7; break;
                case I_m7:
                    score += CH_SC_m7; break;
                case I_M7:
                    score += CH_SC_M7; break;
                default:
                    score += CH_SC_NONCHORD; break;
            }
        }
        if (score > bestFitScore) {
            bestFitRoot = note;
            memcpy(intervals, temp_intervals, 12*sizeof(int));
        }
    } // Now we have the best fit root of all.

    // Determine the type of the chord:
    ChordType type;
    int majorP = 0;
    if(intervals[I_M3] >= intervals[I_m3]) // prefer major over minor
        {majorP = 1;}
    if(majorP) { // Major chords
        if (intervals[I_m7] + intervals[I_M7] > 2) {
            //Yay for magic numbers.  Treat it as a seventh chord only if it's more than a single subdivision
            type = intervals[I_m7] > intervals[I_M7] ? ch_dom_7 : ch_maj_7;
        } else {
            type = ch_maj;
        }
    } else {
        int diminishedP = 0;
        if (intervals[I_d5] > intervals[I_P5]) { // diminished chords
            if (intervals[I_m7] + intervals[I_M6] > 2) { // treat as 7th chord
                type = intervals[I_M6] > intervals[I_m7] ? ch_fulldim_7 : ch_halfdim_7;
            } else {
                type = ch_dim;
            }
        } else { // Minor chords
            // I'm just going to include minor 7th chords... I don't even remember what the other one might be called...
            type = intervals[I_m7] > 2 ? ch_min_7 : ch_min;
        }
    }
    // Done getting type
    int fullChordP;
    int numNonChord = countNonChordTones(intervals, npitches, type, &fullChordP);


    // Ok, stuff what we have so far into the struct
    singleChord->rootNote = bestFitRoot;
    singleChord->type = type;
    singleChord->numNonChordSdivs = numNonChord;
    singleChord->fullChordP = fullChordP;

}


void chordAnalyze_most(Note** parts, ChordInfo* chords) {
    int sdivPitches[nparts*sdivPerDiv];
    for(int measure = 0; measure < nmeasures; ++measure) {
        int sdivPitch = 0;
        for(int beat = 0; beat < mdivision; ++beat) {
            for(int sdiv = 0; sdiv < sdivPerDiv; ++sdiv) {
                for(int partno = 0; partno < nparts; ++partno) {
                    int globalSdivNo = measure*mdivision*sdivPerDiv + beat*sdivPerDiv + sdiv;
                    sdivPitches[sdiv*nparts + partno] = parts[partno][globalSdivNo].pitch;
                }
            }
            // Now all the sdivPitches are in, so analyze what chord is present
            // and stuff it in the chords array
            getChordInfo_most(sdivPitches, nparts*sdivPerDiv, chords+(measure*mdivision+beat));
        }
    }
}


