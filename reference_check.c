/* Independent C reference checks; does not call project cipher modules. */
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

static void caesar_byte(uint8_t *data, size_t len, int key, int decrypt) {
    int shift = key % 256;
    if (shift < 0) shift += 256;
    if (decrypt) shift = (256 - shift) % 256;
    for (size_t i = 0; i < len; ++i) {
        data[i] = (uint8_t)((data[i] + shift) % 256);
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

static void vigenere_bytes(uint8_t *data, size_t len, const uint8_t *key,
                           size_t key_len, size_t *offset, int decrypt) {
    for (size_t i = 0; i < len; ++i) {
        unsigned int k = key[*offset % key_len];
        data[i] = decrypt ? (uint8_t)((data[i] + 256u - k) % 256u)
                          : (uint8_t)((data[i] + k) % 256u);
        *offset = (*offset + 1) % key_len;
    }
}

static int check_text(const char *name, const char *got, const char *expected) {
    if (strcmp(got, expected) == 0) {
        printf("[PASS] %s\n", name);
        return 1;
    }
    printf("[FAIL] %s: got '%s', expected '%s'\n", name, got, expected);
    return 0;
}

int main(void) {
    int passed = 0, total = 0;
    char text[128];

#define CHECK_TEXT(label, expected) do { ++total; passed += check_text(label, text, expected); } while (0)

    strcpy(text, "HELLO"); caesar_letters(text, 3, 0); CHECK_TEXT("Caesar HELLO +3", "KHOOR");
    caesar_letters(text, 3, 1); CHECK_TEXT("Caesar decrypt KHOOR", "HELLO");
    strcpy(text, "XYZ"); caesar_letters(text, 3, 0); CHECK_TEXT("Caesar XYZ +3", "ABC");
    strcpy(text, "ABC"); caesar_letters(text, 26, 0); CHECK_TEXT("Caesar key 26", "ABC");
    strcpy(text, "A A!"); vigenere_letters(text, "B", 0); CHECK_TEXT("Vigenere punctuation", "B B!");
    strcpy(text, "ATTACKATDAWN"); ++total;
    if (vigenere_letters(text, "LEMON", 0)) passed += check_text("Vigenere encrypt", text, "LXFOPVEFRNHR");
    else puts("[FAIL] Vigenere encrypt: invalid key");

    strcpy(text, "LXFOPVEFRNHR"); ++total;
    if (vigenere_letters(text, "LEMON", 1)) passed += check_text("Vigenere decrypt", text, "ATTACKATDAWN");
    else puts("[FAIL] Vigenere decrypt: invalid key");

    {
        uint8_t b = 0xFF;
        caesar_byte(&b, 1, 1, 0);
        ++total;
        if (b == 0x00) { ++passed; puts("[PASS] Caesar byte FF +1"); }
        else puts("[FAIL] Caesar byte FF +1");

        b = 0x00;
        caesar_byte(&b, 1, 255, 0);
        ++total;
        if (b == 0xFF) { ++passed; puts("[PASS] Caesar byte 00 +255"); }
        else puts("[FAIL] Caesar byte 00 +255");
    }

    {
        const uint8_t key[] = {0x01, 0x02};
        uint8_t data[] = {0x00, 0xFF, 0x10};
        const uint8_t expected[] = {0x01, 0x01, 0x11};
        size_t offset = 0;
        vigenere_bytes(data, sizeof data, key, sizeof key, &offset, 0);
        ++total;
        if (memcmp(data, expected, sizeof data) == 0) {
            ++passed; puts("[PASS] Vigenere byte encrypt");
        } else puts("[FAIL] Vigenere byte encrypt");

        offset = 0;
        vigenere_bytes(data, sizeof data, key, sizeof key, &offset, 1);
        const uint8_t original[] = {0x00, 0xFF, 0x10};
        ++total;
        if (memcmp(data, original, sizeof data) == 0) {
            ++passed; puts("[PASS] Vigenere byte decrypt");
        } else puts("[FAIL] Vigenere byte decrypt");
    }

    ++total;
    if (!vigenere_letters(text, "", 0)) {
        ++passed; puts("[PASS] Reject empty Vigenere key");
    } else puts("[FAIL] Reject empty Vigenere key");

    printf("\nResult: %d/%d checks passed.\n", passed, total);
    return passed == total ? 0 : 1;
}
