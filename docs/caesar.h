#ifndef CAESAR_H
#define CAESAR_H

#include <stddef.h>

#define MODE_BYTE  0
#define MODE_ALPHA 1

#define OP_ENCRYPT 0
#define OP_DECRYPT 1

int caesar_process(
    unsigned char *buf,
    size_t n,
    int key,
    size_t offset,
    int op,
    int mode
);

#endif
