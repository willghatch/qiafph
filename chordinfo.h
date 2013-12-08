//chordinfo.h
//
#ifndef CHORDINFO_H
#define CHORDINFO_H

#include "notes.h"

#define I_UN 0
#define I_m2 1
#define I_M2 2
#define I_m3 3
#define I_M3 4
#define I_P4 5
#define I_d5 6
#define I_P5 7
#define I_m6 8
#define I_M6 9
#define I_m7 10
#define I_M7 11

#define CH_SC_UNISON 125
#define CH_SC_m3 75
#define CH_SC_M3 75
#define CH_SC_p5 90
#define CH_SC_d5 30
#define CH_SC_d7 10
#define CH_SC_m7 20
#define CH_SC_M7 30
#define CH_SC_NONCHORD -20


enum ChordTypeEnum {
    ch_nochord, ch_maj, ch_min, ch_dim, ch_aug, ch_maj_7, ch_dom_7, ch_min_7, ch_halfdim_7, ch_fulldim_7, ch_aug_7
};
typedef enum ChordTypeEnum ChordType;

struct ChordInfoStruct {
    int rootNote;
    int inversion;
    ChordType type;
    int fullChordP;
    int doubledNote;
    int numNonChordSdivs;
    int numInPieceKey;
    int inNormalKeyChordsP;
};

typedef struct ChordInfoStruct ChordInfo;



void getChordInfo_most(int* sdivPitches, int npitches, ChordInfo* singleChord);
void chordAnalyze_most(Note** parts, ChordInfo* chords);
void chordAnalyze_putKeyAssociation(ChordInfo* chordInfo, int key);
int numPartsOnNote(int pitch, int mdivNum, Note** parts);
int minorChordP(ChordType t);
int diminishedChordP(ChordType t);
int majorChordP(ChordType t);

#endif //CHORDINFO_H
