// midge_out.c
// for midge output

#include <stdio.h>

#include "globalvars.h"
#include "notes.h"
#include "chordinfo.h"
#include "mutations.h"
#include "score.h"


void outputMidge(Note** parts) {
    FILE* outfile = stdout;

    // Print header
    fprintf(outfile, "@head {\n");
    fprintf(outfile, "\t$time_sig 4/4\n");
    fprintf(outfile, "\t$tempo 120\n");
    fprintf(outfile, "}\n");

    fprintf(outfile, "@body {\n");

    //for (int partno = 0; partno < nparts; ++partno) {
    for (int partno = 3; partno >= 0; --partno) {
        //fprintf(outfile, "\t@channel %d {\n", partno+1);
        fprintf(outfile, "\t@channel 1 {\n" );
        // patch 1 is piano
        fprintf(outfile, "\t\t$patch 1\n");
        for (int gsdiv = 0; gsdiv < totalSubdivisions; ++gsdiv) {
            Note note = parts[partno][gsdiv];
            char* flatmaybe = flat_p(note) ? "-" : "";
            fprintf(outfile, "\t\t/l4/%c%s%d\n", getNoteName_lowercase(note), flatmaybe, getNoteOctave(note));
        }
        fprintf(outfile, "\t}\n");
    }
    // end body
    fprintf(outfile, "}\n");

}

