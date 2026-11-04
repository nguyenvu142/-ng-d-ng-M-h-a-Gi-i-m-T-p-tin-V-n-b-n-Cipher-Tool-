#include "crc32.h"

void crc32_init(crc32_context *ctx) {
    if (ctx == NULL) {
        return;
    }
    ctx->state = UINT32_C(0xFFFFFFFF);
}

void crc32_update(crc32_context *ctx, const void *data, size_t length) {
    if (ctx == NULL || (data == NULL && length != 0)) {
        return;
    }

    const unsigned char *bytes = (const unsigned char *)data;
    uint32_t crc = ctx->state;

    for (size_t i = 0; i < length; ++i) {
        crc ^= (uint32_t)bytes[i];

        for (unsigned int bit = 0; bit < 8; ++bit) {
            if ((crc & 1u) != 0u) {
                crc = (crc >> 1) ^ UINT32_C(0xEDB88320);
            } else {
                crc >>= 1;
            }
        }
    }

    ctx->state = crc;
}

uint32_t crc32_finalize(const crc32_context *ctx) {
    if (ctx == NULL) {
        return 0;
    }
    return ctx->state ^ UINT32_C(0xFFFFFFFF);
}

uint32_t crc32_compute(const void *data, size_t length) {
    crc32_context ctx;
    crc32_init(&ctx);
    crc32_update(&ctx, data, length);
    return crc32_finalize(&ctx);
}
