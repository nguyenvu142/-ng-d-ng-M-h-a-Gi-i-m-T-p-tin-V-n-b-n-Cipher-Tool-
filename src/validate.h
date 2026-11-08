#ifndef VALIDATE_H
#define VALIDATE_H
#include <stdbool.h>

/* Returns false for invalid keys. Warnings are printed for no-op Caesar keys. */
bool validate_caesar_key(int key, bool byte_mode);
bool validate_vigenere_key(const char *key);
#endif
