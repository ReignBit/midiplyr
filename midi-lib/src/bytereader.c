#include "midi/bytereader.h"
#include <string.h>

bool br_can_read(byte_reader_t *br, size_t amount)
{
    return br->pos + amount <= br->size;
}

br_err_t br_read_u8(byte_reader_t *br, uint8_t *out)
{
    if (!br_can_read(br, 1))
    {
        return BR_OOB;
    }
    *out = br->data[br->pos++];
    return BR_OK;
}

br_err_t br_read_u16(byte_reader_t *br, uint16_t *out)
{
    if (!br_can_read(br, 2))
    {
        return BR_OOB;
    }
    *out = (br->data[br->pos] << 8) | br->data[br->pos + 1];
    br->pos += 2;
    return BR_OK;
}

br_err_t br_read_u32(byte_reader_t *br, uint32_t *out)
{
    if (!br_can_read(br, 4))
    {
        return BR_OOB;
    }

    *out = (br->data[br->pos] << 24) |
           (br->data[br->pos + 1] << 16) |
           (br->data[br->pos + 2] << 8) |
           (br->data[br->pos + 3]);

    br->pos += 4;
    return BR_OK;
}

br_err_t br_read_vlq(byte_reader_t *br, uint32_t *out, size_t *out_byte_count)
{

    *out_byte_count = 0;
    *out = 0;
    uint32_t raw = 0;

    while (br->pos < br->size)
    {
        if (!br_can_read(br, 1)) { break; }
        (*out_byte_count)++;
        uint8_t byte, peeked;
        br_read_u8(br, &byte);
        byte &= 0x7F;

        br_peek_from(br, &peeked, br->pos - 1, 1);
        raw = (raw << 8) | peeked;
        *out = (*out << 7) | byte;
        if (!(peeked & 0x80))
        {
            // No more bytes needed
            return BR_OK;
        }
    }
    return BR_OOB;
}

// These strings are NOT null terminated.
br_err_t br_read_str(byte_reader_t *br, char *out, size_t len)
{
    if(!br_can_read(br, len)) { return BR_OOB;}

    memcpy(out, &br->data[br->pos], len);
    br->pos += len;
    return BR_OK;
}

br_err_t br_read_bytes(byte_reader_t *br, uint8_t *out, size_t len)
{
    if(!br_can_read(br, len)) { return BR_OOB;}

    memcpy(out, &br->data[br->pos], len);
    br->pos += len;
    return BR_OK;
}

br_err_t br_peek_from(byte_reader_t *br, uint8_t *out, size_t pos, size_t count)
{
    if (pos + count > br->size) { return BR_OOB; }
    memcpy(out, &br->data[pos], count);
    return BR_OK;
}
br_err_t br_peek(byte_reader_t *br, uint8_t *out)
{
    *out = br->data[br->pos];
    return BR_OK;
}

br_err_t br_seek(byte_reader_t *br, size_t new_pos)
{
    if (new_pos < 0 || new_pos > br->size) { return BR_OOB; }
    br->pos = new_pos;
    return BR_OK;
}