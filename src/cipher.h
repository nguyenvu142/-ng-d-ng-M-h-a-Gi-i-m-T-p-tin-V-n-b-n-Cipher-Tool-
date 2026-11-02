#ifndef CIPHER_H
#define CIPHER_H

#include <stddef.h>
#include <stdint.h>
#include "common.h"

#define MODE_DECRYPT 0
#define MODE_ENCRYPT 1

int caesar_process(unsigned char *buf, size_t len, const Key *key, int mode);
int vigenere_process(unsigned char *buf, size_t len, const Key *key, int mode);
int xor_process(unsigned char *buf, size_t len, const Key *key, int mode);
int process_file(const char *input_filename, const char *output_filename, const Key *key, int mode, int (*cipher_func)(unsigned char *, size_t, const Key *, int));

#endif
