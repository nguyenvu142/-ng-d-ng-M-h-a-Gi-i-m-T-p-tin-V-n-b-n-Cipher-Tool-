#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../crc32.h"

static void test_standard_check_value(void) {
    static const char input[] = "123456789";
    assert(crc32_compute(input, strlen(input)) == UINT32_C(0xCBF43926));
}

static void test_empty_input(void) {
    assert(crc32_compute("", 0) == UINT32_C(0x00000000));
}

static void test_binary_data(void) {
    static const uint8_t input[] = {0x00, 0x01, 0x02, 0xFF, 0x80};
    uint32_t whole = crc32_compute(input, sizeof input);

    crc32_context ctx;
    crc32_init(&ctx);
    crc32_update(&ctx, input, 2);
    crc32_update(&ctx, input + 2, sizeof input - 2);

    assert(crc32_finalize(&ctx) == whole);
}

static void test_single_byte_and_null_zero_length(void) {
    static const uint8_t byte = 0x00;
    crc32_context ctx;
    crc32_init(&ctx);
    crc32_update(&ctx, NULL, 0);
    crc32_update(&ctx, &byte, 1);

    assert(crc32_finalize(&ctx) == crc32_compute(&byte, 1));
}

int main(void) {
    test_standard_check_value();
    test_empty_input();
    test_binary_data();
    test_single_byte_and_null_zero_length();

    puts("CRC32 tests passed (4/4).");
    return 0;
}
