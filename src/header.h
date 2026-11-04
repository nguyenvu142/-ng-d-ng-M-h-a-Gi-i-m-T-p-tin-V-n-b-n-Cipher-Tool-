#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>

#include "common.h"

#define HEADER_SIZE 32
#define FORMAT_VERSION 1

typedef struct {
    uint8_t version;
    uint8_t alg;
    uint16_t flags;
    uint32_t block_n;
    uint64_t orig_size;
    uint32_t crc;
} Header;

void put_u16_le(unsigned char *p, uint16_t v);
void put_u32_le(unsigned char *p, uint32_t v);
void put_u64_le(unsigned char *p, uint64_t v);
uint16_t get_u16_le(const unsigned char *p);
uint32_t get_u32_le(const unsigned char *p);
uint64_t get_u64_le(const unsigned char *p);

ErrCode header_write(FILE *f, const Header *h);
ErrCode header_read(FILE *f, Header *h);

#endif
