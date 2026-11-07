#ifndef CIPHER_H
#define CIPHER_H

#include <stddef.h>
#include <stdint.h>

void caesar_process(uint8_t *data, size_t length, int shift_key, int mode);
void vigenere_process(uint8_t *data, size_t length, const char *text_key, int mode);
void xor_process(uint8_t *data, size_t length, const uint8_t *key, size_t key_len);

/* Cac ham Moc 4: Padding PKCS#7, Hoan vi khoi & CRC32 */
size_t apply_padding(uint8_t *data, size_t length, size_t block_size);
size_t remove_padding(const uint8_t *data, size_t length, size_t block_size);
void transpose_process(uint8_t *data, size_t length, const char *raw_key, int mode);
uint32_t calculate_crc32(const uint8_t *data, size_t length, uint32_t crc_init);

#endif
