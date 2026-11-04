#ifndef UI_H
#define UI_H

#include <stddef.h>

#include "common.h"

ErrCode read_line(char *buf, size_t size);
ErrCode read_int(const char *prompt, int min, int max, int *out);
ErrCode read_key(const char *prompt, Key *key);
int confirm_yn(const char *prompt);

#endif
