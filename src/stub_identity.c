#include "common.h"

/* Ham gia: khong thay doi du lieu, dung de chay thu luong tep. */
ErrCode identity_process(unsigned char *buf, size_t n, const Key *key,
                         uint64_t offset, int op)
{
    (void)buf;
    (void)n;
    (void)offset;
    (void)op;
    return key->len == 0 ? ERR_EMPTY_KEY : ERR_OK;
}
