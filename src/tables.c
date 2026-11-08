#include "tables.h"

uint8_t CAESAR_ENC[256];
uint8_t CAESAR_DEC[256];
char VIGENERE_TABLE[26][26];

/* Generate tables at runtime instead of writing 256/676 entries by hand. */
void tables_init(void) {
    for (int i = 0; i < 256; ++i) {
        CAESAR_ENC[i] = (uint8_t)((i + 1) % 256);
        CAESAR_DEC[i] = (uint8_t)((i + 255) % 256);
    }
    for (int row = 0; row < 26; ++row) {
        for (int col = 0; col < 26; ++col) {
            VIGENERE_TABLE[row][col] = (char)('A' + (row + col) % 26);
        }
    }
}
