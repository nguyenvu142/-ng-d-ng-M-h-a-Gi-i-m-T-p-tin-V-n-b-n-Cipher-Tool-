#include <string.h>
#include "cipher.h"

void vigenere_process(uint8_t *data, size_t length, const char *text_key, int mode) {
    size_t klen = (text_key != NULL) ? strlen(text_key) : 0;
    if (!data || length == 0 || klen == 0) return;
    for (size_t i = 0; i < length; i++) {
        uint8_t k = (uint8_t)text_key[i % klen];
        data[i] = (mode == 1) ? (uint8_t)((data[i] + k) & 0xFF)
                              : (uint8_t)((data[i] - k + 256) & 0xFF);
    }
}
