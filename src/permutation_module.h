#ifndef PERMUTATION_MODULE_H
#define PERMUTATION_MODULE_H

#include <stdint.h>
#include <stddef.h>

// Định nghĩa kích thước khối hoán vị mặc định (ví dụ: 8 phần tử tương đương 8 byte)
#define PERM_MODULE_BLOCK_SIZE 8

void perm_from_key(const uint8_t *key, size_t key_len, uint8_t *perm);

void get_inverse_permutation(const uint8_t *perm, uint8_t *inv_perm, size_t size);

void apply_permutation(const uint8_t *input, uint8_t *output, const uint8_t *perm, size_t size);

#endif 
