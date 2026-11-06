#ifndef ADAPTER_H
#define ADAPTER_H

#include <stddef.h>

#include "common.h"

cipher_fn adapter_lookup(int alg);
uint16_t adapter_flags(int alg);

size_t pad_block(unsigned char *buf, size_t len, size_t n);
int unpad_block(const unsigned char *buf, size_t len, size_t n, size_t *out_len);

#endif
