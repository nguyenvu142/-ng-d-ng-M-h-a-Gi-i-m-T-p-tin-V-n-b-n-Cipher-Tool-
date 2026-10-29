#ifndef FILEINFO_H
#define FILEINFO_H

#include "common.h"

typedef struct {
    char path[512];
    char name[256];
    uint64_t size;
    int exists;
    int readable;
} ThongTinTep;

ErrCode ttt_load(const char *path, ThongTinTep *out);

#endif
