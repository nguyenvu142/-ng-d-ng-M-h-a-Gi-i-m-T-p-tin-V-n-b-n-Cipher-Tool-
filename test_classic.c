#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../caesar.h"
#include "../vigenere.h"
#include "../tables.h"
#include "../validate.h"

static void test_caesar_classic(void) {
    uint8_t text[] = "HELLO";
    caesar_process_letters(text, 5, 3, false);
    assert(memcmp(text, "KHOOR", 5) == 0);
    caesar_process_letters(text, 5, 3, true);
    assert(memcmp(text, "HELLO", 5) == 0);

    uint8_t wrap[] = "XYZ";
    caesar_process_letters(wrap, 3, 3, false);
    assert(memcmp(wrap, "ABC", 3) == 0);

    uint8_t negative[] = "ABC";
    caesar_process_letters(negative, 3, -1, false);
    assert(memcmp(negative, "ZAB", 3) == 0);

    uint8_t noop[] = "ABC";
    caesar_process_letters(noop, 3, 26, false);
    assert(memcmp(noop, "ABC", 3) == 0);
}

static void test_caesar_bytes(void) {
    uint8_t byte = 0xFF;
    caesar_process_bytes(&byte, 1, 1, false);
    assert(byte == 0x00);
    caesar_process_bytes(&byte, 1, 1, true);
    assert(byte == 0xFF);

    byte = 0x00;
    caesar_process_bytes(&byte, 1, 255, false);
    assert(byte == 0xFF);

    uint8_t data[] = {0x10, 0x20, 0x30};
    uint8_t original[] = {0x10, 0x20, 0x30};
    caesar_process_bytes(data, sizeof data, -257, false);
    caesar_process_bytes(data, sizeof data, -257, true);
    assert(memcmp(data, original, sizeof data) == 0);
}

static void test_vigenere_classic(void) {
    uint8_t text[] = "ATTACKATDAWN";
    const char *key = "LEMON";
    size_t offset = 0;
    vigenere_process_letters(text, strlen((char *)text), key,
                             strlen(key), &offset, false);
    assert(memcmp(text, "LXFOPVEFRNHR", 12) == 0);

    offset = 0;
    vigenere_process_letters(text, 12, key, strlen(key), &offset, true);
    assert(memcmp(text, "ATTACKATDAWN", 12) == 0);

    uint8_t punctuation[] = "A A!";
    offset = 0;
    vigenere_process_letters(punctuation, 4, "B", 1, &offset, false);
    assert(memcmp(punctuation, "B B!", 4) == 0);
    assert(offset == 0);

    uint8_t lowercase_key[] = "ATTACK";
    offset = 0;
    vigenere_process_letters(lowercase_key, 6, "lemon", 5, &offset, false);
    assert(memcmp(lowercase_key, "LXFOPV", 6) == 0);
}

static void test_vigenere_bytes(void) {
    const uint8_t key[] = {0x01, 0x02};
    uint8_t data[] = {0x00, 0xFF, 0x10};
    const uint8_t expected[] = {0x01, 0x01, 0x11};
    size_t offset = 0;

    vigenere_process_bytes(data, sizeof data, key, sizeof key, &offset, false);
    assert(memcmp(data, expected, sizeof data) == 0);

    offset = 0;
    vigenere_process_bytes(data, sizeof data, key, sizeof key, &offset, true);
    const uint8_t original[] = {0x00, 0xFF, 0x10};
    assert(memcmp(data, original, sizeof data) == 0);
}

static void test_tables_and_validation(void) {
    tables_init();
    assert(CAESAR_ENC[255] == 0);
    assert(CAESAR_DEC[0] == 255);
    assert(VIGENERE_TABLE[0][0] == 'A');
    assert(VIGENERE_TABLE[1][2] == 'D');

    tables_init_caesar(3);
    assert(CAESAR_ENC['A'] == 'D');
    assert(CAESAR_DEC['D'] == 'A');

    assert(validate_caesar_key(0, true));
    assert(validate_caesar_key(256, true));
    assert(validate_caesar_key(26, false));
    assert(!validate_vigenere_key(""));
    assert(!validate_vigenere_key("LEMON123"));
    assert(!validate_vigenere_key("LEMON!"));
    assert(validate_vigenere_key("LEMON"));
}

int main(void) {
    test_caesar_classic();
    test_caesar_bytes();
    test_vigenere_classic();
    test_vigenere_bytes();
    test_tables_and_validation();
    puts("All tests passed.");
    return 0;
}
