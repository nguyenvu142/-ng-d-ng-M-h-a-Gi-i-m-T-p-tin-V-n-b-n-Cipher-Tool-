#include "validate.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

bool parse_caesar_key(const char *text, int *key) {
    char *end;
    long value = strtol(text, &end, 10);
    if (end == text || *end != '\0') {
        fprintf(stderr, "Loi: khoa Caesar phai la so nguyen tu 1 den 255.\n");
        return false;
    }
    if (value < -1000000L) value = -1000000L;   /* tranh tran so khi ep kieu */
    if (value > 1000000L) value = 1000000L;
    *key = (int)value;
    return true;
}

bool validate_caesar_key(int key, bool byte_mode) {
    if (key < 1 || key > 255) {
        fprintf(stderr, "Loi: khoa Caesar phai la so nguyen tu 1 den 255.\n");
        return false;
    }
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
