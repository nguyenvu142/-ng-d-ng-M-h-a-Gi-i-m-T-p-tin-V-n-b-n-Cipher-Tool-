#include "ui.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void discard_rest(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

ErrCode read_line(char *buf, size_t size)
{
    char *nl;

    if (buf == NULL || size < 2) {
        return ERR_BAD_ARG;
    }
    if (fgets(buf, (int)size, stdin) == NULL) {
        buf[0] = '\0';
        return ERR_EOF;
    }
    nl = strchr(buf, '\n');
    if (nl != NULL) {
        *nl = '\0';
    } else {
        discard_rest();
    }
    return ERR_OK;
}

ErrCode read_int(const char *prompt, int min, int max, int *out)
{
    char line[64];

    for (;;) {
        char *end;
        long v;
        ErrCode e;

        printf("%s [%d-%d]: ", prompt, min, max);
        fflush(stdout);
        e = read_line(line, sizeof line);
        if (e != ERR_OK) {
            return e;
        }
        errno = 0;
        v = strtol(line, &end, 10);
        if (end != line && *end == '\0' && errno == 0 && v >= min && v <= max) {
            *out = (int)v;
            return ERR_OK;
        }
        printf("Gia tri khong hop le, nhap lai.\n");
    }
}

ErrCode read_key(const char *prompt, Key *key)
{
    char line[MAX_KEY_LEN + 1];
    ErrCode e;

    printf("%s: ", prompt);
    fflush(stdout);
    e = read_line(line, sizeof line);
    if (e != ERR_OK) {
        return e;
    }
    key->len = strlen(line);
    memcpy(key->data, line, key->len);
    return key->len == 0 ? ERR_EMPTY_KEY : ERR_OK;
}

int confirm_yn(const char *prompt)
{
    char line[16];

    for (;;) {
        printf("%s (y/n): ", prompt);
        fflush(stdout);
        if (read_line(line, sizeof line) != ERR_OK) {
            return 0;
        }
        if (tolower((unsigned char)line[0]) == 'y' && line[1] == '\0') {
            return 1;
        }
        if (tolower((unsigned char)line[0]) == 'n' && line[1] == '\0') {
            return 0;
        }
    }
}
