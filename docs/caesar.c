#include "caesar.h"
#include "tables.h"

void caesar_process_bytes(uint8_t *data, size_t len, int key, bool decrypt) {
    int shift = ((key % 256) + 256) % 256;
    if (decrypt) shift = (256 - shift) % 256;
    for (size_t i = 0; i < len; ++i) {
        data[i] = (uint8_t)((data[i] + shift) % 256);
    }
}

void caesar_process_letters(uint8_t *data, size_t len, int key, bool decrypt) {
    int shift = ((key % 26) + 26) % 26;
    if (decrypt) shift = (26 - shift) % 26;
    for (size_t i = 0; i < len; ++i) {
        if (data[i] >= 'A' && data[i] <= 'Z') {
            data[i] = (uint8_t)('A' + (data[i] - 'A' + shift) % 26);
        } else if (data[i] >= 'a' && data[i] <= 'z') {
            data[i] = (uint8_t)('a' + (data[i] - 'a' + shift) % 26);
        }
    }
}
