#ifndef PADDING_H
#define PADDING_H

#include <stdint.h>
#include <stddef.h>

#define PADDING_BLOCK_SIZE 8

void pkcs7_pad(const uint8_t *input, size_t input_len, uint8_t *output, size_t *output_len);
int pkcs7_unpad(const uint8_t *input, size_t input_len, size_t *output_len);

#endif