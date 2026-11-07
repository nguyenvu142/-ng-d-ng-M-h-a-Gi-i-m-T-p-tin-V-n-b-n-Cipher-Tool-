#ifndef PADDING_H
#define PADDING_H
#include <stddef.h>
#include <stdint.h>
size_t apply_padding(uint8_t *data, size_t length, size_t block_size);
size_t remove_padding(const uint8_t *data, size_t length, size_t block_size);
#endif
