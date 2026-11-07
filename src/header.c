#include "header.h"

#define MAGIC0 'C'
#define MAGIC1 'T'
#define MAGIC2 'L'
#define MAGIC3 '1'

void put_u16_le(unsigned char *p, uint16_t v)
{
    p[0] = (unsigned char)(v & 0xFFu);
    p[1] = (unsigned char)((v >> 8) & 0xFFu);
}

void put_u32_le(unsigned char *p, uint32_t v)
{
    int i;
    for (i = 0; i < 4; i++) {
        p[i] = (unsigned char)((v >> (8 * i)) & 0xFFu);
    }
}

void put_u64_le(unsigned char *p, uint64_t v)
{
    int i;
    for (i = 0; i < 8; i++) {
        p[i] = (unsigned char)((v >> (8 * i)) & 0xFFu);
    }
}

uint16_t get_u16_le(const unsigned char *p)
{
    return (uint16_t)(p[0] | ((uint16_t)p[1] << 8));
}

uint32_t get_u32_le(const unsigned char *p)
{
    uint32_t v = 0;
    int i;
    for (i = 3; i >= 0; i--) {
        v = (v << 8) | p[i];
    }
    return v;
}

uint64_t get_u64_le(const unsigned char *p)
{
    uint64_t v = 0;
    int i;
    for (i = 7; i >= 0; i--) {
        v = (v << 8) | p[i];
    }
    return v;
}

ErrCode header_write(FILE *f, const Header *h)
{
    unsigned char b[HEADER_SIZE] = {0};

    b[0] = MAGIC0;
    b[1] = MAGIC1;
    b[2] = MAGIC2;
    b[3] = MAGIC3;
    b[4] = h->version;
    b[5] = h->alg;
    put_u16_le(b + 6, h->flags);
    put_u32_le(b + 8, h->block_n);
    put_u64_le(b + 12, h->orig_size);
    put_u32_le(b + 20, h->crc);
    if (fseek(f, 0, SEEK_SET) != 0 || fwrite(b, 1, HEADER_SIZE, f) != HEADER_SIZE) {
        return ERR_IO;
    }
    return ERR_OK;
}

ErrCode header_read(FILE *f, Header *h)
{
    unsigned char b[HEADER_SIZE];

    if (fread(b, 1, HEADER_SIZE, f) != HEADER_SIZE) {
        return ERR_BAD_MAGIC;
    }
    if (b[0] != MAGIC0 || b[1] != MAGIC1 || b[2] != MAGIC2 || b[3] != MAGIC3) {
        return ERR_BAD_MAGIC;
    }
    if (b[4] != FORMAT_VERSION) {
        return ERR_BAD_VERSION;
    }
    h->version = b[4];
    h->alg = b[5];
    h->flags = get_u16_le(b + 6);
    h->block_n = get_u32_le(b + 8);
    h->orig_size = get_u64_le(b + 12);
    h->crc = get_u32_le(b + 20);
    return ERR_OK;
}
