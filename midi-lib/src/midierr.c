#include "midi/midierr.h"

const char *midi_err_str(midi_err_t err)
{
    switch (err)
    {
    case MIDI_OK:                    return "";
    case MIDI_ERR_IO:                return "I/O error";
    case MIDI_ERR_INVALID_FORMAT:    return "Invalid MIDI file";
    case MIDI_ERR_BAD_TRACK:         return "Bad Track - Read alignment bad";
    
    default: return "UNKNOWN ERROR";
    }
}