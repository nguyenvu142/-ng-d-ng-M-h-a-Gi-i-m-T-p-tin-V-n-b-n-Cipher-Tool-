#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 8
#endif

void pkcs7_pad(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (output == NULL || output_len == NULL) {
        return;
    }

    if (input == NULL && input_len > 0) {
        return;
    }

    if (input_len > 0 && input != NULL) {
        memcpy(output, input, input_len);
    }

    uint8_t pad_val = BLOCK_SIZE - (input_len % BLOCK_SIZE);
    for (size_t i = 0; i < pad_val; i++) {
        output[input_len + i] = pad_val;
    }

    *output_len = input_len + pad_val;
}

void pkcs7_unpad(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL || input_len == 0 || input_len % BLOCK_SIZE != 0) {
        return;
    }

    uint8_t pad_val = input[input_len - 1];
    if (pad_val == 0 || pad_val > BLOCK_SIZE || pad_val > input_len) {
        return;
    }

    for (size_t i = 0; i < pad_val; i++) {
        if (input[input_len - 1 - i] != pad_val) {
            return;
        }
    }

    size_t unpadded_len = input_len - pad_val;
    if (unpadded_len > 0) {
        memcpy(output, input, unpadded_len);
    }

    *output_len = unpadded_len;
}
