#ifndef BITOPS_H
#define BITOPS_H

#include <stdint.h>

/* ý nghĩa file: định nghĩa hàm theo các bit cơ bản
GIÚP tăng tính bảo mật
 */
static inline uint8_t rotate_left8(uint8_t value, int shift) {
    shift %= 8;
    if (shift == 0) return value;
    return (value << shift) | (value >> (8 - shift));
}

static inline uint8_t rotate_right8(uint8_t value, int shift) {
    shift %= 8;
    if (shift == 0) return value;
    return (value >> shift) | (value << (8 - shift));
}

#endif // BITOPS_H
