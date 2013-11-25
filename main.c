// mgen.c

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "globalvars.h"
#include "notes.h"
#include "chordinfo.h"



void outputMD();

void putIntervalArray(Note* part, int* intervals) {
    // intervals has to be size of part-1
    int intervalLen = mdivision * sdivPerDiv * nmeasures - 1;
    for(int i = 0; i < intervalLen; ++i) {
        intervals[i] = part[i+1].pitch - part[i].pitch;
    }
}

void partsCp(Note** srcParts, Note** dstParts) {
}
void partsIntervalsCp(int** src, int** dst) {
}
void chordInfoCp(ChordInfo* src, ChordInfo* dst) {
}

int main() {
    initGlobals();
    srand(time(NULL));

    Note **parts, **trialParts;
    int **partsIntervals, **trialPartsIntervals;
    ChordInfo *chordInfo, **trialChordInfo;



    // Initialize parts array and intervals array
    rangemin = malloc(sizeof(int) * nparts);
    rangemax = malloc(sizeof(int) * nparts);
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

    // Generate random notes
    for (int i = 0; i < nparts; ++i) {
        for (int j = 0; j < totalSubdivisions; ++j) {
            int rangediff = rangemax[i] - rangemin[i];
            int p = (rand() % rangediff) + rangemin[i];
            parts[i][j].pitch = p;
            parts[i][j].flags = 0;
        }
    }








    outputMD(parts);
}


void outputMD(Note** parts) {
    // Print header
    printf("\n\n\n"); // first three lines blank
    printf("01/02/13 Me\n"); // <Date> <Name of encoder>
    printf("wk#:1 mvn:1\n"); // work and movement number
    printf("sourcey source\n"); // source
    printf("A work title could go here\n"); // work title
    printf("1 0\n"); // movement title
    //printf("This part\n"); // name of part
    //printf("\n"); // misc designations - I don't know what they are
    printf("group memberships: sound, score\n"); // group memberships
    printf("sound: part 1 of 1\n"); // <name1>: part <x> of <number in group>
    printf("score: part 1 of 1\n"); // <name1>: part <x> of <number in group>
    printf("$  K:0  Q:2  T:4/4  C1:4\n");
    // K: key
    // Q: divisions per quarter note
    // T: time signature
    // Cn: Clef n type

    // print body
    for (int m = 1; m <= nmeasures; ++m) {
        printf("measure %d\n", m);
        for (int part = 0; part < nparts; ++part) {
            if (part != 0) {
                printf("back 8\n");
            }
            for (int sd = 0; sd < mdivision*sdivPerDiv; ++sd) {
                Note n = parts[part][m*mdivision*sdivPerDiv+sd];
                //printf("Note: %c %d\n", getNoteName(parts[3][i]), getNoteOctave(parts[3][i]));
                char* flatstr = flat_p(n) ? "f" : "";
                char* postNoteStr = flat_p(n) ? "" : " ";
                printf("%c%s%d%s   1\n", getNoteName(n), flatstr, getNoteOctave(n), postNoteStr); 
                // Col 1-4 note
                // col 5 blank
                // col 6-8 duration (right justified), 9 tie mark
            }
        }
    }

    // print ending stuff
    printf("/END");
    // No ending newline!
}

