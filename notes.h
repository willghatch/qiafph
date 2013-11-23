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

struct notestruct {
    int pitch;
    int flags; 
        // TIED, REST, IMMUTABLE...
};

typedef struct notestruct Note;

char getNoteName(Note n);
int flat_p(Note n);
int getNoteOctave(Note n);

#endif //NOTES_H
