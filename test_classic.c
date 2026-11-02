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
}

static void test_vigenere_classic(void) {
    uint8_t text[] = "ATTACKATDAWN";
    const char *key = "LEMON";
    size_t offset = 0;
    vigenere_process_letters(text, strlen((char *)text), key, strlen(key), &offset, false);
    assert(memcmp(text, "LXFOPVEFRNHR", 12) == 0);
    offset = 0;
    vigenere_process_letters(text, 12, key, strlen(key), &offset, true);
    assert(memcmp(text, "ATTACKATDAWN", 12) == 0);
}

static void test_caesar_byte_wrap(void) {
    uint8_t byte = 0xFF;
    caesar_process_bytes(&byte, 1, 1, false);
    assert(byte == 0x00);
    caesar_process_bytes(&byte, 1, 1, true);
    assert(byte == 0xFF);
}

static void test_tables_and_validation(void) {
    tables_init();
    assert(CAESAR_ENC[255] == 0);
    assert(CAESAR_DEC[0] == 255);
    assert(VIGENERE_TABLE[0][0] == 'A');
    assert(VIGENERE_TABLE[1][2] == 'D');
    assert(validate_caesar_key(0, true));
    assert(validate_caesar_key(26, false));
    assert(!validate_vigenere_key(""));
    assert(validate_vigenere_key("LEMON"));
}

int main(void) {
    test_caesar_classic();
    test_vigenere_classic();
    test_caesar_byte_wrap();
    test_tables_and_validation();
    puts("All tests passed.");
    return 0;
}
