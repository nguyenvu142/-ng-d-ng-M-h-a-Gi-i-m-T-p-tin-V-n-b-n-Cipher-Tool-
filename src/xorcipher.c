#include <string.h>
#include "cipher.h"

void xor_process(uint8_t *data, size_t length, const uint8_t *key, size_t key_len) {
    if (!data || !key || key_len == 0) return;
    for (size_t i = 0; i < length; i++) {
        data[i] ^= key[i % key_len];
    }
}

/* PKCS#7 Padding: Luon chen tu 1..block_size bytes de tranh cat nham du lieu that */
size_t apply_padding(uint8_t *data, size_t length, size_t block_size) {
    if (!data || block_size == 0 || block_size > 255) return length;
    size_t pad_len = block_size - (length % block_size);
    for (size_t i = 0; i < pad_len; i++) {
        data[length + i] = (uint8_t)pad_len;
    }
    return length + pad_len;
}

size_t remove_padding(const uint8_t *data, size_t length, size_t block_size) {
    if (!data || length == 0 || block_size == 0 || (length % block_size) != 0) return length;
    uint8_t pad_len = data[length - 1];
    if (pad_len == 0 || pad_len > block_size || pad_len > length) return length;
    for (size_t i = 0; i < pad_len; i++) {
        if (data[length - 1 - i] != pad_len) return length;
    }
    return length - pad_len;
}

void transpose_process(uint8_t *data, size_t length, const char *raw_key, int mode) {
    size_t klen = (raw_key != NULL) ? strlen(raw_key) : 0;
    if (!data || length == 0 || klen == 0) return;

    uint8_t key_mask = 0;
    for (size_t k = 0; k < klen; k++) {
        key_mask = (uint8_t)((key_mask + (uint8_t)raw_key[k] * (k + 1)) & 0xFF);
    }

    if (mode == 1) {
        for (size_t base = 0; base + 16 <= length; base += 16) {
            for (size_t j = 0; j < 8; j++) {
                uint8_t tmp = data[base + j];
                data[base + j] = data[base + 15 - j];
                data[base + 15 - j] = tmp;
            }
        }
        for (size_t i = 0; i < length; i++) {
            uint8_t k = (uint8_t)(raw_key[i % klen] ^ key_mask);
            data[i] = (uint8_t)((data[i] + k) & 0xFF);
        }
    } else {
        for (size_t i = 0; i < length; i++) {
            uint8_t k = (uint8_t)(raw_key[i % klen] ^ key_mask);
            data[i] = (uint8_t)((data[i] - k + 256) & 0xFF);
        }
        for (size_t base = 0; base + 16 <= length; base += 16) {
            for (size_t j = 0; j < 8; j++) {
                uint8_t tmp = data[base + j];
                data[base + j] = data[base + 15 - j];
                data[base + 15 - j] = tmp;
            }
        }
    }
}
