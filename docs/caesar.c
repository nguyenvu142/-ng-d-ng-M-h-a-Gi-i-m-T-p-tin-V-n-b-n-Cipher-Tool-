#include "cipher.h"

void caesar_process(uint8_t *data, size_t length, int shift_key, int mode) {
    if (!data || length == 0) return;
    int s = ((shift_key % 256) + 256) % 256;
    for (size_t i = 0; i < length; i++) {
        data[i] = (mode == 1) ? (uint8_t)((data[i] + s) & 0xFF)
                              : (uint8_t)((data[i] - s + 256) & 0xFF);
    }
}
