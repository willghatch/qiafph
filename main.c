// mgen.c

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "globalvars.h"
#include "notes.h"
#include "chordinfo.h"
#include "mutations.h"
#include "score.h"



void outputMD();
void outputMidge();

void putIntervalArray(Note* part, int* intervals) {
    // intervals has to be size of part-1
    int intervalLen = mdivision * sdivPerDiv * nmeasures - 1;
    for(int i = 0; i < intervalLen; ++i) {
        intervals[i] = part[i+1].pitch - part[i].pitch;
    }
}

void partsCp(Note** srcParts, Note** dstParts) {
    for(int i = 0; i < nparts; ++i) {
        memcpy(dstParts[i], srcParts[i], sizeof(Note)*totalSubdivisions);
    }
}
void partsIntervalsCp(int** src, int** dst) {
    for(int i = 0; i < nparts; ++i) {
        memcpy(dst[i], src[i], sizeof(int)*totalSubdivisions);
    }
}
void chordInfoCp(ChordInfo* src, ChordInfo* dst) {
    memcpy(dst, src, sizeof(ChordInfo)*nmeasures*mdivision);
}

int main() {
    initGlobals();
    srand(time(NULL));

    Note **parts, **trialParts;
    int **partsIntervals, **trialPartsIntervals;
    ChordInfo *chordInfo, *trialChordInfo;
    int key = 0; // key of C for now...



    // Initialize parts array and intervals array
    rangemin = malloc(sizeof(int) * nparts);
    rangemax = malloc(sizeof(int) * nparts);
    rangediff = malloc(sizeof(int) * nparts);
    chordInfo = malloc(sizeof(ChordInfo) * nmeasures * mdivision);
    trialChordInfo = malloc(sizeof(ChordInfo) * nmeasures * mdivision);
    parts = malloc(sizeof(Note*) * (nparts + 1));
    trialParts = malloc(sizeof(Note*) * (nparts + 1));
    partsIntervals = malloc(sizeof(int*) * (nparts+1));
    trialPartsIntervals = malloc(sizeof(int*) * (nparts+1));
    for (int i = 0; i < nparts; ++i) {
        parts[i] = malloc(sizeof(Note) * totalSubdivisions);
        trialParts[i] = malloc(sizeof(Note) * totalSubdivisions);
        partsIntervals[i] = malloc(sizeof(int)  * totalSubdivisions);
        trialPartsIntervals[i] = malloc(sizeof(int)  * totalSubdivisions);
    }
    parts[nparts] = 0;
    trialParts[nparts] = 0;
    partsIntervals[nparts] = 0;
    trialPartsIntervals[nparts] = 0;

// bass range: E2 (24+4=28) to C4 (48)
// tenor range: D3 (36+2=38) to G4 (48+7=55)
// alto: G3 (36+7=43) to D5 (60+2=62)
// Soprano: D4 (48+2=50) to G5 (60+7=67)
    rangemin[0] = 28;
    rangemin[1] = 38;
    rangemin[2] = 43;
    rangemin[3] = 50;
    
    rangemax[0] = 48;
    rangemax[1] = 55;
    rangemax[2] = 62;
    rangemax[3] = 67;

    // I think I got the ranges off... I'm going to boost them a little
    for(int i = 0; i < 4; ++i) {
        rangemin[i] += 12;
        rangemax[i] += 12;
    }

    for (int i = 0; i < nparts; ++i) {
        rangediff[i] = rangemax[i] - rangemin[i];
    }

    // Generate random notes
    for (int i = 0; i < nparts; ++i) {
        for (int j = 0; j < totalSubdivisions; ++j) {
            int p = (rand() % rangediff[i]) + rangemin[i];
            parts[i][j].pitch = p;
            parts[i][j].flags = 0;
        }
    }

    // do analysis
    for(int i = 0; i < nparts; ++i) {
        putIntervalArray(parts[i], partsIntervals[i]);
    }
    chordAnalyze_most(parts, chordInfo);
    // TODO - find best fit key, maybe
    chordAnalyze_putKeyAssociation(chordInfo, key);
    pieceScore = scorePiece(parts, chordInfo, partsIntervals);

    // copy to trial versions
    partsCp(parts, trialParts);
    chordInfoCp(chordInfo, trialChordInfo);
    partsIntervalsCp(partsIntervals, trialPartsIntervals);



    int acceptedChangeP = 1;
    double temperature = 5000;
    double coolRate = 1;
    double takechanceThresh = 1;
    int numPasses = 20000;
    int numAccepted = 0;
    int numRejected = 0;

    for(int i = 0; i < numPasses; ++i) {
        acceptedChangeP = 0;
        mutatePiece(trialParts);
        for(int i = 0; i < nparts; ++i) {
            putIntervalArray(trialParts[i], trialPartsIntervals[i]);
        }
        chordAnalyze_most(trialParts, trialChordInfo);
        chordAnalyze_putKeyAssociation(trialChordInfo, key);

        long trialScore = scorePiece(trialParts, trialChordInfo, trialPartsIntervals);

        int scoreDiff = trialScore - pieceScore;
        if (scoreDiff > 0) {
            acceptedChangeP = 1;
        } else {
            double takechance = -scoreDiff / temperature;
            acceptedChangeP = takechance < takechanceThresh ? 1 : 0;
        }

        if (acceptedChangeP) {
            pieceScore = trialScore;
            partsCp(trialParts, parts);
            chordInfoCp(trialChordInfo, chordInfo);
            partsIntervalsCp(trialPartsIntervals, partsIntervals);
            ++numAccepted;
        } else {
            partsCp(parts, trialParts);
            chordInfoCp(chordInfo, trialChordInfo);
            partsIntervalsCp(partsIntervals, trialPartsIntervals);
            ++numRejected;
        }

        // Cool down
        temperature -= coolRate;
        if (temperature < 0) {
            temperature = 0;
        }

    }
    
    // Copy current accepted state into trial state
    partsCp(parts, trialParts);
    chordInfoCp(chordInfo, trialChordInfo);
    partsIntervalsCp(partsIntervals, trialPartsIntervals);


    //outputMD(parts);
    outputMidge(parts);

    fprintf(stderr, "Piece Score: %d\n", pieceScore);
    fprintf(stderr, "Acceptance rate: %f\n", (double)numAccepted / numPasses);
}



