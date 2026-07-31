#pragma once
#include "midi/event/event_midi.h"

typedef struct {
    int element_size;
    int num_elements;
    int max_elements;
    midi_event_t *data;
} midi_event_vector_t;

void midi_vector_init(midi_event_vector_t *vec);
void midi_vector_push(midi_event_vector_t *vec, midi_event_t *data);