// notes.h
#ifndef NOTES_H
#define NOTES_H


#define NPITCHES 12
#define CNOTE 0
#define DbNOTE 1
#define DNOTE 2
#define EbNOTE 3
#define ENOTE 4
#define FNOTE 5
#define GbNOTE 6
#define GNOTE 7
#define AbNOTE 8
#define ANOTE 9
#define BbNOTE 10
#define BNOTE 11

#define C0 0


#define KP_DO 0
#define KP_RE 2
#define KP_MI 4
#define KP_FA 5
#define KP_SOL 7
#define KP_LA 9
#define KP_TI 11
// chromatic ones
#define KP_RA 1
#define KP_ME 3
#define KP_SE 6
#define KP_LE 8
#define KP_TE 10

struct notestruct {
    int pitch;
    int flags; 
        // TIED, REST, IMMUTABLE...
};

typedef struct notestruct Note;

char getNoteName(Note n);
char getNoteName_lowercase(Note n);
int flat_p(Note n);
int getNoteOctave(Note n);
int leadingToneP(int pitch, int key);
int tonicNoteP(int pitch, int key);
int noteInKeyP(Note n, int key);
int stepwiseInKeyP(int p1, int p2, int key);

#endif //NOTES_H
