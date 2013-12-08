// rulescores.h

#ifndef RULESCORES_H
#define RULESCORES_H


// Header file for the configuration of rule weights


////  Chord Rules
#define R_NO_CHORD_ON_BEAT -100
#define R_NON_CHORD_SDIV -10
#define R_CHORD_IN_NORMAL_KEY_CHORDS 100
#define R_FULL_CHORD 200

#define R_START_CHORD__I 300
#define R_START_CHORD__V 250

#define R_END_CADENCE__AUTHENTIC 400
#define R_END_CADENCE__PLAGAL 350 

#define R_DOUBLE_ROOT 40
#define R_DOUBLE_THIRD -20
#define R_DOUBLE_FIFTH 0


// Chord progressions
#define R_CHORD_PROG__I_IV 200
#define R_CHORD_PROG__I_V 50
#define R_CHORD_PROG__I_ii 20
#define R_CHORD_PROG__I_vi 40

#define R_CHORD_PROG__ii_V 40
#define R_CHORD_PROG__ii_viid 40

#define R_CHORD_PROG__iii_vi 20
#define R_CHORD_PROG__iii_IV 20

#define R_CHORD_PROG__IV_V 150
#define R_CHORD_PROG__IV_viid 30
#define R_CHORD_PROG__IV_ii 20
#define R_CHORD_PROG__IV_I 50

#define R_CHORD_PROG__V_I 200
#define R_CHORD_PROG__V_vi 50

#define R_CHORD_PROG__vi_IV 30
#define R_CHORD_PROG__vi_V 40
#define R_CHORD_PROG__vi_ii 30

#define R_CHORD_PROG__viid_I 60
#define R_CHORD_PROG__viid_V 40




#define R_VOICE_CROSS -50
#define R_OCTAVE_DISTANCE_NOT_BASS -40



////  Melody Rules
#define R_STEPWISE_IN_KEY 60
#define R_LEADING_TONE_NOT_TO_TONIC -100
#define R_NOTE_NOT_IN_KEY -100




#endif //RULESCORES_H

