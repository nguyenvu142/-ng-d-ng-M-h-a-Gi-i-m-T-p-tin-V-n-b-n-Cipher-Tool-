#include "fileinfo.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
typedef struct _stati64 stat_t;
#define STAT_FN _stati64
#define IS_DIR(m) (((m) & _S_IFDIR) != 0)
#else
typedef struct stat stat_t;
#define STAT_FN stat
#define IS_DIR(m) S_ISDIR(m)
#endif

static const char *base_name(const char *path)
{
    const char *p = path + strlen(path);
    while (p > path && p[-1] != '/' && p[-1] != '\\') {
        p--;
    }
    return p;
}

static void copy_str(char *dst, size_t cap, const char *src)
{
    size_t len = strlen(src);
    if (len >= cap) {
        len = cap - 1;
    }
    memcpy(dst, src, len);
    dst[len] = '\0';
}

static int can_read(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return 0;
    }
    fclose(f);
    return 1;
}

ErrCode ttt_load(const char *path, ThongTinTep *out)
{
    stat_t st;

    if (path == NULL || out == NULL || path[0] == '\0') {
        return ERR_BAD_ARG;
    }
    memset(out, 0, sizeof *out);
    copy_str(out->path, sizeof out->path, path);
    copy_str(out->name, sizeof out->name, base_name(path));
    if (STAT_FN(path, &st) != 0) {
        return ERR_NOT_FOUND;
    }
    out->exists = 1;
    if (IS_DIR(st.st_mode)) {
        return ERR_IS_DIR;
    }
    out->size = (uint64_t)st.st_size;
    out->readable = can_read(path);
    return out->readable ? ERR_OK : ERR_NO_READ;
}
