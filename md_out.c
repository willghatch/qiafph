//md_out.c
// musedata format output...

#include <stdio.h>

#include "globalvars.h"
#include "notes.h"
#include "chordinfo.h"
#include "mutations.h"
#include "score.h"

void outputMD(Note** parts, char* file) {
    FILE* outfile = fopen(file, "w");

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

