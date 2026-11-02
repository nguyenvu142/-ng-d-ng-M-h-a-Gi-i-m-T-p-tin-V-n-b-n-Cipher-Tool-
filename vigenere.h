#ifndef VIGENERE_H
#define VIGENERE_H
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Byte mode: adds/subtracts key bytes modulo 256, using offset as key position. */
void vigenere_process_bytes(uint8_t *data, size_t len, const uint8_t *key,
                            size_t key_len, size_t *offset, bool decrypt);
/* Classic mode: Vigenere for ASCII letters; non-letters do not consume key. */
void vigenere_process_letters(uint8_t *data, size_t len, const char *key,
                              size_t key_len, size_t *offset, bool decrypt);
#endif
