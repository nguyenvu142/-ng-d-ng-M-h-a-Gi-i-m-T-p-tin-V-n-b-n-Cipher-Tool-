#ifndef TABLES_H
#define TABLES_H
#include <stdint.h>

extern uint8_t CAESAR_ENC[256];
extern uint8_t CAESAR_DEC[256];
extern char VIGENERE_TABLE[26][26];

void tables_init(void);
#endif
