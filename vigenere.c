#include "vigenere.h"

void vigenere_process_bytes(uint8_t *data, size_t len, const uint8_t *key,
                            size_t key_len, size_t *offset, bool decrypt) {
    if (!data || !key || key_len == 0 || !offset) return;
    for (size_t i = 0; i < len; ++i) {
        unsigned int k = key[*offset % key_len];
        if (decrypt) data[i] = (uint8_t)((data[i] + 256u - k) % 256u);
        else data[i] = (uint8_t)((data[i] + k) % 256u);
        *offset = (*offset + 1) % key_len;
    }
}

void vigenere_process_letters(uint8_t *data, size_t len, const char *key,
                              size_t key_len, size_t *offset, bool decrypt) {
    if (!data || !key || key_len == 0 || !offset) return;
    for (size_t i = 0; i < len; ++i) {
        int base;
        if (data[i] >= 'A' && data[i] <= 'Z') base = 'A';
        else if (data[i] >= 'a' && data[i] <= 'z') base = 'a';
        else continue;

        unsigned char kc = (unsigned char)key[*offset % key_len];
        int k = (kc >= 'a' && kc <= 'z') ? kc - 'a' : kc - 'A';
        int value = (int)data[i] - base;
        value = decrypt ? (value - k + 26) % 26 : (value + k) % 26;
        data[i] = (uint8_t)(base + value);
        *offset = (*offset + 1) % key_len;
    }
}
