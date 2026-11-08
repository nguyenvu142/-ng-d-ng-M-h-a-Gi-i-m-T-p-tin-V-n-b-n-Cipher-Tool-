#include <stddef.h>
#include <stdint.h>
#include <string.h>

void pkcs7_pad(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    uint8_t pad_val = (uint8_t)(8 - (input_len % 8));
    if (input != output) {
        memcpy(output, input, input_len);
    }
    for (size_t i = 0; i < pad_val; i++) {
        output[input_len + i] = pad_val;
    }
    *output_len = input_len + pad_val;
}

void pkcs7_unpad(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input_len == 0) {
        *output_len = (size_t)-1;
        return;
    }
    uint8_t pad_val = input[input_len - 1];
    if (pad_val == 0 || pad_val > 8 || pad_val > input_len) {
        *output_len = (size_t)-1;
        return;
    }
    for (size_t i = 0; i < pad_val; i++) {
        if (input[input_len - 1 - i] != pad_val) {
            *output_len = (size_t)-1;
            return;
        }
    }
    size_t orig_len = input_len - pad_val;
    memcpy(output, input, orig_len);
    *output_len = orig_len;
}
