#ifndef TRANSPOSE_PROCESS_H
#define TRANSPOSE_PROCESS_H

#include <stddef.h>
#include <stdint.h>

int encrypt_transpose_block(const unsigned char *in, unsigned char *out, size_t len, const unsigned char *key, size_t key_len);
int decrypt_transpose_block(const unsigned char *in, unsigned char *out, size_t len, const unsigned char *key, size_t key_len);

#endif
