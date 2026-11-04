#ifndef CRC32_H
#define CRC32_H

#include <stddef.h>
#include <stdint.h>

/*
 * CRC-32/ISO-HDLC (CRC-32/ADCCP)
 * Reflected polynomial: 0xEDB88320
 * init: 0xFFFFFFFF, xorout: 0xFFFFFFFF
 *
 * The update API supports streaming data in multiple chunks.
 */
typedef struct {
    uint32_t state;
} crc32_context;

void crc32_init(crc32_context *ctx);
void crc32_update(crc32_context *ctx, const void *data, size_t length);
uint32_t crc32_finalize(const crc32_context *ctx);
uint32_t crc32_compute(const void *data, size_t length);

#endif
