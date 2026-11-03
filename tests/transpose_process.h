#ifndef TRANSPOSE_PROCESS_H
#define TRANSPOSE_PROCESS_H

#include <stdint.h>
#include <stddef.h>

#define TRANSPOSE_BLOCK_SIZE 8

void transpose_block(const uint8_t *input, uint8_t *output, const uint8_t *perm);
void inverse_transpose_block(const uint8_t *input, uint8_t *output, const uint8_t *inv_perm);

#endif
