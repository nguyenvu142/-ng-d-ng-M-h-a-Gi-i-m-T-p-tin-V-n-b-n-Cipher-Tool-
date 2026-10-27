#ifndef TABLES_H
#define TABLES_H

#define BYTE_TABLE_SIZE 256
#define ALPHA_TABLE_SIZE 26

extern unsigned char caesar_table[BYTE_TABLE_SIZE];
extern unsigned char caesar_inverse_table[BYTE_TABLE_SIZE];

extern unsigned char vigenere_table[ALPHA_TABLE_SIZE][ALPHA_TABLE_SIZE];

void tables_init(void);
void build_caesar_tables(int shift);

#endif
