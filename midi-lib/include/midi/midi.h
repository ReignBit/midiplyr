#pragma once
#include "midierr.h"
#include "midi/midieventvector.h"
#include "event/event_midi.h"

typedef enum {
    MIDI_FMT_SINGLE,
    MIDI_FMT_MULTI_TRACK,
    MIDI_FMT_MULTI_SONG
} midi_format_t;


typedef struct {
    midi_event_vector_t events;
    uint16_t num_events;
    size_t capacity;
} midi_track_t;

typedef struct {
    uint16_t division;
    midi_format_t track_format;
    
    midi_track_t *tracks;
    uint16_t num_tracks;

} midi_file_t;

midi_err_t midi_file_load(const char *filepath, midi_file_t *out);
const char *midi_get_format_name(midi_format_t format);