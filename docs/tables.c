#include "tables.h"

unsigned char caesar_table[BYTE_TABLE_SIZE];
unsigned char caesar_inverse_table[BYTE_TABLE_SIZE];

unsigned char vigenere_table[ALPHA_TABLE_SIZE][ALPHA_TABLE_SIZE];

void build_caesar_tables(int shift)
{
    int i;

    shift = ((shift % 256) + 256) % 256;

    for (i = 0; i < 256; i++) {
        caesar_table[i] = (unsigned char)((i + shift) % 256);
        caesar_inverse_table[i] =
            (unsigned char)((i - shift + 256) % 256);
    }
}

void tables_init(void)
{
    int row;
    int col;

    build_caesar_tables(0);

    for (row = 0; row < 26; row++) {
        for (col = 0; col < 26; col++) {
            vigenere_table[row][col] =
                (unsigned char)((row + col) % 26);
        }
    }
}
