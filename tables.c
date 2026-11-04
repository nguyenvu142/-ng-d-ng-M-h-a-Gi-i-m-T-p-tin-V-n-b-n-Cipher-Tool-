#include "tables.h"

uint8_t CAESAR_ENC[256];
uint8_t CAESAR_DEC[256];
char VIGENERE_TABLE[26][26];

void tables_init_caesar(int key) {
    int shift = key % 256;
    if (shift < 0) shift += 256;

    for (int i = 0; i < 256; ++i) {
        CAESAR_ENC[i] = (uint8_t)((i + shift) % 256);
        CAESAR_DEC[i] = (uint8_t)((i - shift + 256) % 256);
    }
}

void tables_init(void) {
    tables_init_caesar(1);

    for (int row = 0; row < 26; ++row) {
        for (int col = 0; col < 26; ++col) {
            VIGENERE_TABLE[row][col] = (char)('A' + (row + col) % 26);
        }
    }
}
