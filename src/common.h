#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdint.h>

#define BLOCK_SIZE 65536
#define MAX_KEY_LEN 256
#define MAX_PAD 256
#define DEFAULT_BLOCK_N 8
#define OP_ENCRYPT 0
#define OP_DECRYPT 1
#define FLAG_PADDED 0x0001
#define FLAG_ALPHA 0x0002

enum ErrCode {
    ERR_OK = 0,
    ERR_IO,
    ERR_NOT_FOUND,
    ERR_NO_READ,
    ERR_IS_DIR,
    ERR_BAD_MAGIC,
    ERR_BAD_VERSION,
    ERR_BAD_CRC,
    ERR_BAD_ARG,
    ERR_EMPTY_KEY,
    ERR_SAME_FILE,
    ERR_NOMEM,
    ERR_EOF
};
typedef enum ErrCode ErrCode;

enum AlgId { ALG_CAESAR = 1, ALG_VIGENERE = 2, ALG_XOR = 3, ALG_TRANSPOSE = 4 };

typedef struct {
    unsigned char data[MAX_KEY_LEN];
    size_t len;
} Key;

typedef ErrCode (*cipher_fn)(unsigned char *buf, size_t n, const Key *key,
                             uint64_t offset, int op);
typedef cipher_fn (*cipher_lookup)(int alg);

const char *err_msg(ErrCode e);

#endif
