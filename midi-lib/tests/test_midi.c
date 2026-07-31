#include "midi/midi.h"
#include <stdio.h>

int main(int argc, const char *argv[]) {
    
    if (argc <= 1) { printf("Usage: %s <midi file>\n", argv[0]); return 0; }

    midi_file_t midi;

    printf("MIDI TEST LAND\n");

    printf("Loading midi file '%s'\n", argv[1]);
    return midi_file_load(argv[1], &midi);
}