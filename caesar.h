#ifndef CAESAR_H
#define CAESAR_H
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Byte mode: every byte is shifted modulo 256, including 0xFF -> 0x00. */
void caesar_process_bytes(uint8_t *data, size_t len, int key, bool decrypt);
/* Letter mode: shifts only ASCII A-Z/a-z; other bytes are unchanged. */
void caesar_process_letters(uint8_t *data, size_t len, int key, bool decrypt);
#endif
