#include "caesar.h"
#include "tables.h"

void caesar_process_bytes(uint8_t *data, size_t len, int key, bool decrypt) {
    if (data == NULL || len == 0) return;

    /* Build a key-specific lookup table, then perform one lookup per byte. */
    tables_init_caesar(key);
    const uint8_t *table = decrypt ? CAESAR_DEC : CAESAR_ENC;

    for (size_t i = 0; i < len; ++i) {
        data[i] = table[data[i]];
    }
}

void caesar_process_letters(uint8_t *data, size_t len, int key, bool decrypt) {
    if (data == NULL || len == 0) return;

    int shift = key % 26;
    if (shift < 0) shift += 26;
    if (decrypt) shift = (26 - shift) % 26;

    for (size_t i = 0; i < len; ++i) {
        if (data[i] >= 'A' && data[i] <= 'Z') {
            data[i] = (uint8_t)('A' + (data[i] - 'A' + shift) % 26);
        } else if (data[i] >= 'a' && data[i] <= 'z') {
            data[i] = (uint8_t)('a' + (data[i] - 'a' + shift) % 26);
        }
    }
}
