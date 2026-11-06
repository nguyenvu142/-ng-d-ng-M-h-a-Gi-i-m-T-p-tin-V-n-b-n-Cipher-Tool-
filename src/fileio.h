#ifndef FILEIO_H
#define FILEIO_H

#include "common.h"

uint32_t crc32_update(uint32_t crc, const unsigned char *buf, size_t n);

ErrCode process_file(const char *in_path, const char *out_path, int alg,
                     uint16_t flags, uint32_t block_n, const Key *key,
                     cipher_fn fn);

ErrCode decrypt_file(const char *in_path, const char *out_path,
                     const Key *key, cipher_lookup lookup);

#endif
