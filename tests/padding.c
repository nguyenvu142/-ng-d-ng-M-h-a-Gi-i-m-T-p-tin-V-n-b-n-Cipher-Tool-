#include "padding.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pkcs7_pad(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len) {
    if (input == NULL || output == NULL || output_len == NULL) return;

    memcpy(output, input, input_len);
    uint8_t pad_value = PADDING_BLOCK_SIZE - (input_len % PADDING_BLOCK_SIZE);

    for (size_t i = 0; i < pad_value; i++) {
        output[input_len + i] = pad_value;
    }

    *output_len = input_len + pad_value;
}

int pkcs7_unpad(const uint8_t *input, size_t input_len, size_t *output_len) {
    if (input == NULL || output_len == NULL || input_len == 0) return 0;
    if (input_len % PADDING_BLOCK_SIZE != 0) return 0;

    uint8_t pad_value = input[input_len - 1];

    if (pad_value < 1 || pad_value > PADDING_BLOCK_SIZE) {
        return 0;
    }

    for (size_t i = input_len - pad_value; i < input_len; i++) {
        if (input[i] != pad_value) {
            return 0;
        }
    }

    *output_len = input_len - pad_value;
    return 1;
}

int main() {
    const char *plaintext = "Hello UTC2 BTL"; 
    size_t orig_len = strlen(plaintext);
    printf("[>] Original (%d bytes): \"%s\"\n", (int)orig_len, plaintext);

    size_t padded_max_len = orig_len + PADDING_BLOCK_SIZE;
    uint8_t *padded_buffer = (uint8_t *)malloc(padded_max_len);
    
    size_t actual_padded_len = 0;
    pkcs7_pad((const uint8_t *)plaintext, orig_len, padded_buffer, &actual_padded_len);

    printf("[+] Padded   (%d bytes): ", (int)actual_padded_len);
    for (size_t i = 0; i < actual_padded_len; i++) {
        if (padded_buffer[i] >= 32 && padded_buffer[i] <= 126) {
            printf("'%c'  ", padded_buffer[i]);
        } else {
            printf("0x%02X ", padded_buffer[i]);
        }
    }
    printf("\n");

    size_t unpadded_len = 0;
    int unpad_status = pkcs7_unpad(padded_buffer, actual_padded_len, &unpadded_len);

    if (unpad_status == 1) {
        char *restored_text = (char *)malloc(unpadded_len + 1);
        memcpy(restored_text, padded_buffer, unpadded_len);
        restored_text[unpadded_len] = '\0';

        printf("[<] Unpadded (%d bytes): \"%s\"\n", (int)unpadded_len, restored_text);
        
        if (strcmp(plaintext, restored_text) == 0 && unpadded_len == orig_len) {
            printf("[SUCCESS]\n");
        } else {
            printf("[FAILED]\n");
        }
        free(restored_text);
    } else {
        printf("[ERROR]\n");
    }

    free(padded_buffer);
    return 0;
}
