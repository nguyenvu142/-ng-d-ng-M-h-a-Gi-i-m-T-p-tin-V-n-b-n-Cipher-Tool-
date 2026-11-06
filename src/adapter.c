#include "adapter.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "caesar.h"
#include "permutation_module.h"
#include "team_padding.h"
#include "transpose_process.h"
#include "vigenere.h"

void xor_encrypt(const uint8_t *input, uint8_t *output, size_t length,
                 const uint8_t *key, size_t key_len);
void xor_decrypt(const uint8_t *input, uint8_t *output, size_t length,
                 const uint8_t *key, size_t key_len);

static ErrCode caesar_adapter(unsigned char *buf, size_t n, const Key *key,
                              uint64_t offset, int op)
{
    char text[MAX_KEY_LEN + 1];

    (void)offset;
    memcpy(text, key->data, key->len);
    text[key->len] = '\0';
    caesar_process_bytes(buf, n, atoi(text), op == OP_DECRYPT);
    return ERR_OK;
}

static ErrCode vigenere_adapter(unsigned char *buf, size_t n, const Key *key,
                                uint64_t offset, int op)
{
    size_t pos = (size_t)(offset % key->len);

    vigenere_process_bytes(buf, n, key->data, key->len, &pos, op == OP_DECRYPT);
    return ERR_OK;
}

static ErrCode xor_adapter(unsigned char *buf, size_t n, const Key *key,
                           uint64_t offset, int op)
{
    unsigned char rot[MAX_KEY_LEN];
    size_t start = (size_t)(offset % key->len);
    size_t j;

    for (j = 0; j < key->len; j++) {
        rot[j] = key->data[(start + j) % key->len];
    }
    if (op == OP_ENCRYPT) {
        xor_encrypt(buf, buf, n, rot, key->len);
    } else {
        xor_decrypt(buf, buf, n, rot, key->len);
    }
    return ERR_OK;
}

static ErrCode transpose_adapter(unsigned char *buf, size_t n, const Key *key,
                                 uint64_t offset, int op)
{
    uint8_t perm[PERM_MODULE_BLOCK_SIZE];
    uint8_t inv[PERM_MODULE_BLOCK_SIZE];
    uint8_t tmp[PERM_MODULE_BLOCK_SIZE];
    size_t i;

    (void)offset;
    if (n % PERM_MODULE_BLOCK_SIZE != 0) {
        return ERR_BAD_ARG;
    }
    perm_from_key(key->data, key->len, perm);
    get_inverse_permutation(perm, inv, PERM_MODULE_BLOCK_SIZE);
    for (i = 0; i < n; i += PERM_MODULE_BLOCK_SIZE) {
        if (op == OP_ENCRYPT) {
            transpose_block(buf + i, tmp, perm);
        } else {
            inverse_transpose_block(buf + i, tmp, inv);
        }
        memcpy(buf + i, tmp, PERM_MODULE_BLOCK_SIZE);
    }
    return ERR_OK;
}

cipher_fn adapter_lookup(int alg)
{
    switch (alg) {
    case ALG_CAESAR: return caesar_adapter;
    case ALG_VIGENERE: return vigenere_adapter;
    case ALG_XOR: return xor_adapter;
    case ALG_TRANSPOSE: return transpose_adapter;
    default: return NULL;
    }
}

uint16_t adapter_flags(int alg)
{
    return alg == ALG_TRANSPOSE ? FLAG_PADDED : 0;
}

size_t pad_block(unsigned char *buf, size_t len, size_t n)
{
    size_t keep = len - (len % n);
    uint8_t tmp[2 * PADDING_BLOCK_SIZE];
    size_t out = 0;

    (void)n;
    pkcs7_pad(buf + keep, len - keep, tmp, &out);
    memcpy(buf + keep, tmp, out);
    return keep + out;
}

int unpad_block(const unsigned char *buf, size_t len, size_t n, size_t *out_len)
{
    (void)n;
    return pkcs7_unpad(buf, len, out_len) ? 0 : -1;
}
