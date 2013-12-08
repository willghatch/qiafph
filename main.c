// mgen.c

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "globalvars.h"
#include "notes.h"
#include "chordinfo.h"
#include "mutations.h"
#include "score.h"



void outputMD();
void outputMidge();
void outputAnalysis();

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

void printHelp(int exitStatus) {
    fprintf(stderr, "qiafph -- qiafph is a four part harmonizer\n");
    fprintf(stderr, "Written by William Hatch for CS 5600 at USU\n");
    fprintf(stderr, "Generates four part harmony.  Configured at the moment in source\n");
    fprintf(stderr, "in rulescores.h and globalvars.h\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "-m <file>  --midge <file>      Midge output file\n");
//    fprintf(stderr, "-M <file>  --musedata <file>   MuseData output file\n");
    fprintf(stderr, "-a <file>  --analysis <file>   Analysis info output file\n");
    fprintf(stderr, "-n <int>   --nparts <int>      Number of parts (default 4, must be > 2)\n");
    fprintf(stderr, "-v         --verbose           Print some extra info to stderr\n");
    fprintf(stderr, "-h         --help              Print this message to stderr\n");
    fprintf(stderr, "\nA music output format (midge or musedata) must be supplied\n");
    exit(exitStatus);
}

int main(int argc, char** argv) {
    
    char* midgeOutFile = NULL;
    char* museDataOutFile = NULL;
    char* analysisOutFile = NULL;
    verboseP = 0;
    nparts = 4;
    int printHelpP = 0;

    struct option long_options[] =
        {
            {"midge",     required_argument,  0,   'm'},
//            {"musedata",  required_argument,  0,   'M'},
            {"analysis",  required_argument,  0,   'a'},
            {"nparts",    required_argument,  0,   'n'},
            {"help",      no_argument,        0,   'h'},
            {"verbose",   no_argument,        0,   'v'},
            {0,0,0,0}
        };

    // Get command line options
    while (1) {
        int c;
        int option_index = 0;
        c = getopt_long(argc, argv, "vhn:m:a:", long_options, &option_index);

        if (c == -1) {
            break;
        }

        switch(c) {
            case 'h':
                printHelpP = 1;
                break;
            case 'm':
                midgeOutFile = optarg;
                break;
//            case 'M':
//                museDataOutFile = optarg;
            case 'a':
                analysisOutFile = optarg;
                break;
            case 'v':
                verboseP = 1;;
                break;
            case 'n':
                nparts = atoi(optarg);
                if (nparts < 2) {printHelp(1);}
                break;
            case '?':
                printHelp(1);
                break;
            default:
                printHelp(1);
                break;
        }
    }

    if(printHelpP) {
        printHelp(0);
    }

    if (museDataOutFile == NULL && midgeOutFile == NULL) {
        printHelp(1);
    }



    // Initialize
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
    rangemax[0] = 48;
    rangemin[1] = 38;
    rangemax[1] = 55;
    if (nparts > 2) {
        rangemin[2] = 43;
        rangemax[2] = 62;
    }
    if (nparts > 3) {
        rangemin[3] = 50;
        rangemax[3] = 67;
    }
    if (nparts > 4) {
        for(int i = 4; i < nparts; ++i) {
            rangemin[i] = rangemin[i-1] + 10;
            rangemax[i] = rangemax[i-1] + 10;
        }
    }
    

    // I think I got the ranges off... I'm going to boost them a little
    if (nparts <= 4) {
        for(int i = 0; i < nparts; ++i) {
            rangemin[i] += 12;
            rangemax[i] += 12;
    }
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
    int numPasses = 40000;
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


    if (museDataOutFile != NULL) {
        outputMD(parts, museDataOutFile);
    }
    if (midgeOutFile != NULL) {
        outputMidge(parts, midgeOutFile);
    }
    if (analysisOutFile != NULL) {
        outputAnalysis(analysisOutFile, parts, chordInfo);
    }

    if (verboseP) {
        fprintf(stderr, "Piece Score: %d\n", pieceScore);
        fprintf(stderr, "Acceptance rate: %f\n", (double)numAccepted / numPasses);
    }
}



