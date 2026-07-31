#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    BR_OK = 0,
    BR_OOB
} br_err_t;

typedef struct {
    const uint8_t *data;
    size_t size;
    size_t pos;
} byte_reader_t;

bool br_can_read(byte_reader_t *br, size_t amount);
br_err_t br_read_u8(byte_reader_t *br, uint8_t *out);
br_err_t br_read_u16(byte_reader_t *br, uint16_t *out);
br_err_t br_read_u32(byte_reader_t *br, uint32_t *out);
br_err_t br_read_vlq(byte_reader_t *br, uint32_t *out, size_t *out_byte_count);
br_err_t br_read_str(byte_reader_t *br, char *out, size_t len);
br_err_t br_read_bytes(byte_reader_t *br, uint8_t *out, size_t len);
br_err_t br_peek_from(byte_reader_t *br, uint8_t *out, size_t pos, size_t count);
br_err_t br_peek(byte_reader_t *br, uint8_t *out);
br_err_t br_seek(byte_reader_t *br, size_t new_pos);


