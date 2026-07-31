#include "midi/midieventvector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void midi_vector_init(midi_event_vector_t *vec)
{
    vec->data = malloc(vec->element_size * vec->max_elements);
    printf("allocated vec to ptr: %p\n", (void *)vec->data);
}

void midi_vector_push(midi_event_vector_t *vec, midi_event_t *data)
{
    if (vec->num_elements >= vec->max_elements)
    {
        // Array is full, we need to realloc more space!
        vec->max_elements += 20;
        vec->data = realloc(vec->data, vec->element_size * vec->max_elements);
    }

    memcpy(
        &vec->data[vec->num_elements],
        data,
        sizeof(midi_event_t)
    );
    vec->num_elements++;
}