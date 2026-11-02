#include "validate.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool validate_caesar_key(int key, bool byte_mode) {
    int modulus = byte_mode ? 256 : 26;
    int normalized = ((key % modulus) + modulus) % modulus;
    if (normalized == 0) {
        printf("Canh bao: khoa Caesar khong lam thay doi du lieu.\n");
    }
    return true;
}

bool validate_vigenere_key(const char *key) {
    if (key == NULL || key[0] == '\0') {
        fprintf(stderr, "Loi: khoa Vigenere khong duoc rong.\n");
        return false;
    }
    for (size_t i = 0; key[i] != '\0'; ++i) {
        if (!isalpha((unsigned char)key[i]) || (unsigned char)key[i] > 127) {
            fprintf(stderr, "Loi: khoa Vigenere chi duoc chua chu cai A-Z.\n");
            return false;
        }
    }
    return true;
}
