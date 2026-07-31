#include "midi/midi.h"
#include "midi/bytereader.h"
#include "midi/midieventvector.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static size_t file_size(FILE *file)
{
    fpos_t pos;
    fgetpos(file, &pos);

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fsetpos(file, &pos);
    return size;
}

static bool is_valid(byte_reader_t *br, size_t pos, const char *magic)
{
    if (!br_can_read(br, 4))
    {
        return false;
    }
    br->pos += 4; // Consume the magic number

    return memcmp(br->data + pos, magic, 4) == 0;
}

static midi_err_t parse(byte_reader_t *br, midi_file_t *midi)
{
    // Check if we are looking at a midi file.
    if (!is_valid(br, 0, "MThd"))
    {
        return MIDI_ERR_INVALID_FORMAT;
    }
    br_seek(br, br->pos + 4); // Skip header length as is always 6

    // Parse the header chunk
    uint16_t format;
    br_read_u16(br, &format);
    printf("MIDI File is %s\n", midi_get_format_name(midi->track_format));
    if (format > MIDI_FMT_MULTI_SONG)
    {
        return MIDI_ERR_INVALID_FORMAT;
    }
    midi->track_format = (midi_format_t)format;

    br_read_u16(br, &midi->num_tracks);
    midi->tracks = malloc(sizeof(midi_track_t) * midi->num_tracks);
    printf("MIDI File has %d tracks (%ld bytes allocated)\n",
           midi->num_tracks,
           sizeof(midi_track_t) * midi->num_tracks);

    br_read_u16(br, &midi->division);
    printf("MIDI File time division is %d\n", midi->division);

    // Parse the track chunk(s)
    int i = 0;
    while (br->pos != br->size)
    {
        printf("br->pos = %ld\n", br->pos);

        // Check it is the start of a chunk
        if (!is_valid(br, br->pos, "MTrk")) { return MIDI_ERR_BAD_TRACK; }
        printf("> Parsing Track\n");


        uint32_t length;
        br_read_u32(br, &length);
        printf("> \tTrack is %d bytes\n", length);
        
        midi_track_t *curr_track = &midi->tracks[i];

        // Allocate some starting memory for events
        midi->tracks[i].events = (midi_event_vector_t) {
            .element_size = sizeof(midi_event_t),
            .num_elements = 0,
            .max_elements = 200,
            .data = NULL
        };

        midi_vector_init(&midi->tracks[i].events);

        // Each track event
        size_t track_end = br->pos + length;
        while (br->pos < track_end)
        {
            midi_event_t event;
            size_t bytes_read = 0;
            br_read_vlq(br, &event.delta_time, &bytes_read);

            uint8_t status_byte;
            br_read_u8(br, &status_byte);
            //printf("STATUS BYTE: %X\n", status_byte);

            switch (status_byte & 0xF0)
            {
            case 0x80:
                // Note Off
                printf("NOTE OFF\n");
                event.type = EVENT_MIDI;
                event.midi.channel = status_byte & 0x0F;
                event.midi.type = MIDI_NOTE_OFF;
                br_read_u8(br, &event.midi.note.note);
                br_read_u8(br, &event.midi.note.velocity);
                break;
            case 0x90:
                // Note on
                printf("NOTE ON\n");
                event.type = EVENT_MIDI;
                event.midi.channel = status_byte & 0x0F;
                event.midi.type = MIDI_NOTE_ON;
                br_read_u8(br, &event.midi.note.note);
                br_read_u8(br, &event.midi.note.velocity);
                break;
            case 0xA0:
                // Polyphonic
                printf("POLYPHONIC PRESSURE\n");
                event.type = EVENT_MIDI;
                event.midi.channel = status_byte & 0x0F;
                event.midi.type = MIDI_POLY_PRESSURE;
                br_read_u8(br, &event.midi.note.note);
                br_read_u8(br, &event.midi.note.velocity);
                break;
            case 0xB0:
                // Controller
                printf("CONTROLLER\n");
                event.type = EVENT_MIDI;
                event.midi.channel = status_byte & 0x0F;
                event.midi.type = MIDI_CONTROL_CHANGE;
                br_read_u8(br, &event.midi.controller.controller);
                br_read_u8(br, &event.midi.controller.value);
                break;
            case 0xC0:
                // Program Change
                printf("PROGRAM CHANGE\n");
                event.type = EVENT_MIDI;
                event.midi.channel = status_byte & 0x0F;
                event.midi.type = MIDI_PROGRM_CHANGE;
                br_read_u8(br, &event.midi.program.program);
                break;
            case 0xD0:
                // Channel pressure
                printf("CHANNEL PRESSURE\n");
                event.type = EVENT_MIDI;
                event.midi.channel = status_byte & 0x0F;
                event.midi.type = MIDI_CHANNEL_PRESSURE;
                br_read_u8(br, &event.midi.channel_pressure.pressure);
                break;
            case 0xE0:
                // Pitch Bend
                printf("PITCH BEND\n");
                event.type = EVENT_MIDI;
                event.midi.channel = status_byte & 0x0F;
                event.midi.type = MIDI_PITCH_BEND;
                br_read_u8(br, &event.midi.bend.lsb);
                br_read_u8(br, &event.midi.bend.msb);
                break;

            case 0xF0:
                switch (status_byte & 0x0F)
                {
                    case 0x00:
                    case 0x07:
                        // SysEx message
                        printf("UNSUPPORTED SYSEX!");
                        break;

                    case 0x0F:
                        // Meta
                        printf("META\n");
                        event.type = EVENT_META;
                        br_read_u8(br, &event.meta.raw_type);

                        switch (event.meta.raw_type)
                        {   
                        case 0x00:
                            event.meta.type = META_SEQUENCE_NUMBER;
                            br_seek(br, br->pos + 1); // ignore 0x02
                            br_read_u16(br, &event.meta.sequence_number);
                            break;
                        
                        case 0x01:
                        case 0x02:
                        case 0x03:
                        case 0x04:
                        case 0x05:
                        case 0x06:
                        case 0x07:
                        case 0x08:
                        case 0x09:
                            event.meta.type = (event_type_t)event.meta.raw_type;
                            br_read_vlq(br, &event.meta.length, &bytes_read);
                            event.meta.data = br->data + br->pos;
                            br_seek(br, br->pos + event.meta.length);

                        default:
                            break;
                        }

                        
                        break;    
                }
                break;
            }
            midi_vector_push(&midi->tracks[i].events, &event);
        }
        printf("Loaded track %d, %d events (%ld bytes / %ld unused)\n",
            i,
            midi->tracks[i].events.num_elements,
            midi->tracks[i].events.num_elements * sizeof(midi_event_t),
            (midi->tracks[i].events.max_elements - midi->tracks[i].events.num_elements) * sizeof(midi_event_t)
        );

        
        i++;
        //br_seek(br, br->pos length);
    }

    return MIDI_OK;
}

const char *midi_get_format_name(midi_format_t fmt)
{
    switch (fmt)
    {
    case MIDI_FMT_SINGLE:
        return "Single Track Mode";
    case MIDI_FMT_MULTI_TRACK:
        return "Multi Track Mode";
    case MIDI_FMT_MULTI_SONG:
        return "Multi Song Mode";
    default:
        return "Unsupported Format";
    }
}

midi_err_t midi_file_load(const char *filepath, midi_file_t *out)
{
    FILE *file = fopen(filepath, "rb");
    if (file == NULL)
    {
        return MIDI_ERR_IO;
    }

    size_t length = file_size(file);

    printf("Filepath : %s\n", filepath);
    printf("   Bytes : %ld\n", length);

    uint8_t *data = malloc(length);
    fread(data, 1, length, file);
    fclose(file);

    byte_reader_t br = {
        .data = data,
        .size = length,
        .pos = 0};

    midi_err_t err = parse(&br, out);
    if (err != MIDI_OK)
    {
        printf("Error: Failed parsing midi file, reason: %s\n", midi_err_str(err));
        return err;
    }

    return MIDI_OK;
}