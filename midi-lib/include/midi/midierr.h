#pragma once

typedef enum {
    MIDI_OK = 0,
    MIDI_ERR_IO,
    MIDI_ERR_INVALID_FORMAT,
    MIDI_ERR_BAD_TRACK
} midi_err_t;


const char *midi_err_str(midi_err_t err);
