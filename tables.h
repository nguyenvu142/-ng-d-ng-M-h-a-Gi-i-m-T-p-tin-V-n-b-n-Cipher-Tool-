#ifndef TABLES_H
#define TABLES_H

#include <stdint.h>

/* Caesar lookup tables: input byte -> output byte for the active key. */
extern uint8_t CAESAR_ENC[256];
extern uint8_t CAESAR_DEC[256];

/* Vigenere lookup table: row = key shift, column = input letter A-Z. */
extern char VIGENERE_TABLE[26][26];

void tables_init(void);
void tables_init_caesar(int key);

#endif
