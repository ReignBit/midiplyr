#pragma once
#include <stdint.h>
#include <stdlib.h>

typedef enum {
    EVENT_MIDI,
    EVENT_SYSEX,
    EVENT_META
} event_type_t;

typedef enum {
    MIDI_NOTE_OFF,
    MIDI_NOTE_ON,
    MIDI_POLY_PRESSURE,
    MIDI_CONTROL_CHANGE,
    MIDI_PROGRM_CHANGE,
    MIDI_CHANNEL_PRESSURE,
    MIDI_PITCH_BEND
} midi_message_type_t;

typedef enum {
    META_SEQUENCE_NUMBER,
    META_TEXT,
    META_COPYRIGHT,
    META_TRACK_NAME,
    META_INSTRUMENT_NAME,
    META_LYRIC,
    META_MARKER,
    META_CUE_POINT,
    META_PROGRAM_NAME,
    META_DEVICE_NAME,
    META_MIDI_CHANNEL_PREFIX,
    META_MIDI_PORT,
    META_EOT,
    META_TEMPO,
    META_SMPTE_OFFSET,
    META_TIME_SIGNATURE,
    META_KEY_SIGNATURE,
    META_SEQ_SPECIFIC
} meta_message_type_t;

typedef struct {

    midi_message_type_t type;
    uint8_t status;
    uint8_t channel;

    union {
        struct {
            uint8_t note;
            uint8_t velocity; // Also pressure when Status & 0xA
        } note;

        struct {
            uint8_t controller;
            uint8_t value;
        } controller;

        struct {
            uint8_t program;
        } program;

        struct {
            uint8_t pressure;
        } channel_pressure;

        struct {
            uint8_t lsb, msb;
        } bend;
    };
} midi_event_type_midi_t;

typedef struct {
    uint32_t delta_time;
    event_type_t type;

    union {
        midi_event_type_midi_t midi;

        struct {
            uint8_t raw_type;
            meta_message_type_t type;
            uint32_t length;
            uint8_t *data;
            uint16_t sequence_number;
        } meta;

        struct {
            uint32_t length;
            uint8_t *data;
        } sysex;
    };
} midi_event_t;