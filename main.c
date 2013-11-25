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




    outputMD(parts);
    fprintf(stderr, "Piece Score: %d\n", pieceScore);
    fprintf(stderr, "Acceptance rate: %f\n", (double)numAccepted / numPasses);
}


void outputMD(Note** parts) {
    FILE* outfile = stdout;

    // Print header
    fprintf(outfile, "\n\n\n"); // first three lines blank
    fprintf(outfile, "01/02/13 Me\n"); // <Date> <Name of encoder>
    fprintf(outfile, "wk#:1 mvn:1\n"); // work and movement number
    fprintf(outfile, "sourcey source\n"); // source
    fprintf(outfile, "A work title could go here\n"); // work title
    fprintf(outfile, "1 0\n"); // movement title
    //fprintf(outfile, "This part\n"); // name of part
    //fprintf(outfile, "\n"); // misc designations - I don't know what they are
    fprintf(outfile, "group memberships: sound, score\n"); // group memberships
    fprintf(outfile, "sound: part 1 of 1\n"); // <name1>: part <x> of <number in group>
    fprintf(outfile, "score: part 1 of 1\n"); // <name1>: part <x> of <number in group>
    fprintf(outfile, "$  K:0  Q:2  T:4/4  C1:4\n");
    // K: key
    // Q: divisions per quarter note
    // T: time signature
    // Cn: Clef n type

    // print body
    for (int m = 1; m <= nmeasures; ++m) {
        fprintf(outfile, "measure %d\n", m);
        for (int part = 0; part < nparts; ++part) {
            if (part != 0) {
                fprintf(outfile, "back 8\n");
            }
            for (int sd = 0; sd < mdivision*sdivPerDiv; ++sd) {
                Note n = parts[part][m*mdivision*sdivPerDiv+sd];
                //fprintf(outfile, "Note: %c %d\n", getNoteName(parts[3][i]), getNoteOctave(parts[3][i]));
                char* flatstr = flat_p(n) ? "f" : "";
                char* postNoteStr = flat_p(n) ? "" : " ";
                fprintf(outfile, "%c%s%d%s   1\n", getNoteName(n), flatstr, getNoteOctave(n), postNoteStr); 
                // Col 1-4 note
                // col 5 blank
                // col 6-8 duration (right justified), 9 tie mark
            }
        }
    }

    // print ending stuff
    fprintf(outfile, "/END");
    // No ending newline!
}

