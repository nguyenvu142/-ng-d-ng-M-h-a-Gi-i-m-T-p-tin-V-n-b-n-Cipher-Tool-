/* Chuong trinh C kiem tra doc lap, khong goi caesar.c/vigenere.c. */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static void caesar_letters(char *s, int key, int decrypt) {
    int shift = key % 26;
    if (shift < 0) shift += 26;
    if (decrypt) shift = (26 - shift) % 26;
    for (size_t i = 0; s[i] != '\0'; ++i) {
        unsigned char c = (unsigned char)s[i];
        if (c >= 'A' && c <= 'Z') s[i] = (char)('A' + (c - 'A' + shift) % 26);
        else if (c >= 'a' && c <= 'z') s[i] = (char)('a' + (c - 'a' + shift) % 26);
    }
}

static int vigenere_letters(char *s, const char *key, int decrypt) {
    size_t n = strlen(key), pos = 0;
    if (n == 0) return 0;
    for (size_t i = 0; s[i] != '\0'; ++i) {
        unsigned char c = (unsigned char)s[i];
        int base, kch = (unsigned char)key[pos % n], k;
        if (c >= 'A' && c <= 'Z') base = 'A';
        else if (c >= 'a' && c <= 'z') base = 'a';
        else continue;
        if (kch >= 'A' && kch <= 'Z') k = kch - 'A';
        else if (kch >= 'a' && kch <= 'z') k = kch - 'a';
        else return 0;
        int v = (int)c - base;
        v = decrypt ? (v - k + 26) % 26 : (v + k) % 26;
        s[i] = (char)(base + v);
        ++pos;
    }
    return 1;
}

static int check(const char *name, const char *got, const char *expected) {
    if (strcmp(got, expected) == 0) {
        printf("[PASS] %s\n", name);
        return 1;
    }
    printf("[FAIL] %s: nhan '%s', can '%s'\n", name, got, expected);
    return 0;
}

int main(void) {
    int passed = 0, total = 0;
    char s[128];

#define CHECK_TEXT(label, expected) do { ++total; passed += check(label, s, expected); } while (0)

    strcpy(s, "HELLO"); caesar_letters(s, 3, 0); CHECK_TEXT("Caesar HELLO +3", "KHOOR");
    caesar_letters(s, 3, 1); CHECK_TEXT("Caesar giai ma KHOOR", "HELLO");
    strcpy(s, "XYZ"); caesar_letters(s, 3, 0); CHECK_TEXT("Caesar XYZ +3", "ABC");
    strcpy(s, "ABC"); caesar_letters(s, 26, 0); CHECK_TEXT("Caesar khoa 26 khong doi", "ABC");
    strcpy(s, "ATTACKATDAWN"); ++total;
    if (vigenere_letters(s, "LEMON", 0)) passed += check("Vigenere ma hoa", s, "LXFOPVEFRNHR");
    else puts("[FAIL] Vigenere ma hoa: khoa khong hop le");
    strcpy(s, "LXFOPVEFRNHR"); ++total;
    if (vigenere_letters(s, "LEMON", 1)) passed += check("Vigenere giai ma", s, "ATTACKATDAWN");
    else puts("[FAIL] Vigenere giai ma: khoa khong hop le");
    {
        uint8_t b = 0xFF;
        b = (uint8_t)((b + 1u) % 256u);
        ++total;
        if (b == 0x00) { ++passed; puts("[PASS] Caesar byte FF +1 = 00"); }
        else puts("[FAIL] Caesar byte FF +1 = 00");
    }
    ++total;
    if (!vigenere_letters(s, "", 0)) { ++passed; puts("[PASS] Tu choi khoa Vigenere rong"); }
    else puts("[FAIL] Tu choi khoa Vigenere rong");

    printf("\nKet qua: %d/%d vector dat.\n", passed, total);
    return passed == total ? 0 : 1;
}
