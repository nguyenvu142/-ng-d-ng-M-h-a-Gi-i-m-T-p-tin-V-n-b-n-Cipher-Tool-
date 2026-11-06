#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileinfo.h"
#include "header.h"
#include "adapter.h"

uint32_t crc32_update(uint32_t crc, const unsigned char *buf, size_t n)
{
    static uint32_t table[256];
    static int ready = 0;
    size_t i;
    int k;

    if (!ready) {
        for (i = 0; i < 256; i++) {
            uint32_t c = (uint32_t)i;
            for (k = 0; k < 8; k++) {
                c = (c & 1u) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            }
            table[i] = c;
        }
        ready = 1;
    }
    crc = ~crc;
    for (i = 0; i < n; i++) {
        crc = table[(crc ^ buf[i]) & 0xFFu] ^ (crc >> 8);
    }
    return ~crc;
}

static void show_progress(uint64_t done, uint64_t total)
{
    int pct = total == 0 ? 100 : (int)((done * 100u) / total);
    if (pct > 100) {
        pct = 100;
    }
    fprintf(stderr, "\rTien do: %3d%%", pct);
    if (pct == 100) {
        fputc('\n', stderr);
    }
}

static int same_path(const char *a, const char *b)
{
    return strcmp(a, b) == 0;
}

static ErrCode check_block_n(uint32_t n)
{
    if (n == 0 || n > 255 || BLOCK_SIZE % n != 0) {
        return ERR_BAD_ARG;
    }
    return ERR_OK;
}

static ErrCode finish_output(FILE *fi, FILE *fo, const char *tmp,
                             const char *out_path, ErrCode e)
{
    if (fi != NULL) {
        fclose(fi);
    }
    if (fo != NULL && fclose(fo) != 0 && e == ERR_OK) {
        e = ERR_IO;
    }
    if (e == ERR_OK) {
        remove(out_path);
        if (rename(tmp, out_path) != 0) {
            e = ERR_IO;
        }
    }
    if (e != ERR_OK) {
        remove(tmp);
    }
    return e;
}

static ErrCode encrypt_loop(FILE *fi, FILE *fo, Header *h, const Key *key,
                            cipher_fn fn, unsigned char *buf)
{
    uint64_t done = 0;
    uint32_t crc = 0;
    int last = 0;
    ErrCode e = ERR_OK;

    while (!last && e == ERR_OK) {
        size_t n = fread(buf, 1, BLOCK_SIZE, fi);
        size_t read_n = n;
        if (n < BLOCK_SIZE) {
            if (ferror(fi)) {
                return ERR_IO;
            }
            last = 1;
        }
        crc = crc32_update(crc, buf, n);
        if (last && (h->flags & FLAG_PADDED)) {
            n = pad_block(buf, n, h->block_n);
        }
        if (n > 0) {
            e = fn(buf, n, key, done, OP_ENCRYPT);
        }
        if (e == ERR_OK && n > 0 && fwrite(buf, 1, n, fo) != n) {
            e = ERR_IO;
        }
        done += read_n;
        show_progress(done, h->orig_size);
    }
    h->crc = crc;
    return e;
}

ErrCode process_file(const char *in_path, const char *out_path, int alg,
                     uint16_t flags, uint32_t block_n, const Key *key,
                     cipher_fn fn)
{
    ThongTinTep ti;
    Header h;
    FILE *fi, *fo;
    unsigned char *buf;
    char tmp[600];
    ErrCode e;

    if (key == NULL || fn == NULL) {
        return ERR_BAD_ARG;
    }
    if (key->len == 0) {
        return ERR_EMPTY_KEY;
    }
    e = check_block_n(block_n);
    if (e != ERR_OK) {
        return e;
    }
    e = ttt_load(in_path, &ti);
    if (e != ERR_OK) {
        return e;
    }
    if (same_path(in_path, out_path)) {
        return ERR_SAME_FILE;
    }
    snprintf(tmp, sizeof tmp, "%s.tmp", out_path);
    fi = fopen(in_path, "rb");
    fo = fopen(tmp, "wb");
    buf = malloc(BLOCK_SIZE + MAX_PAD);
    if (fi == NULL || fo == NULL || buf == NULL) {
        free(buf);
        return finish_output(fi, fo, tmp, out_path,
                             buf == NULL ? ERR_NOMEM : ERR_IO);
    }
    h.version = FORMAT_VERSION;
    h.alg = (uint8_t)alg;
    h.flags = flags;
    h.block_n = block_n;
    h.orig_size = ti.size;
    h.crc = 0;
    e = header_write(fo, &h);
    if (e == ERR_OK) {
        fseek(fo, HEADER_SIZE, SEEK_SET);
        e = encrypt_loop(fi, fo, &h, key, fn, buf);
    }
    if (e == ERR_OK) {
        e = header_write(fo, &h);
    }
    free(buf);
    return finish_output(fi, fo, tmp, out_path, e);
}

typedef struct {
    uint64_t cipher_total;
    uint64_t plain_out;
    uint32_t crc;
} DecState;

static ErrCode decrypt_loop(FILE *fi, FILE *fo, const Header *h,
                            const Key *key, cipher_fn fn, unsigned char *buf,
                            DecState *st)
{
    uint64_t done = 0;
    ErrCode e = ERR_OK;

    while (done < st->cipher_total && e == ERR_OK) {
        size_t n = fread(buf, 1, BLOCK_SIZE, fi);
        size_t m;
        if (n == 0) {
            return ferror(fi) ? ERR_IO : ERR_BAD_CRC;
        }
        e = fn(buf, n, key, done, OP_DECRYPT);
        done += n;
        m = n;
        if (e == ERR_OK && done == st->cipher_total && (h->flags & FLAG_PADDED)
            && unpad_block(buf, n, h->block_n, &m) != 0) {
            return ERR_BAD_CRC;
        }
        st->crc = crc32_update(st->crc, buf, m);
        if (e == ERR_OK && m > 0 && fwrite(buf, 1, m, fo) != m) {
            e = ERR_IO;
        }
        st->plain_out += m;
        show_progress(done, st->cipher_total);
    }
    return e;
}

static ErrCode open_for_decrypt(const char *in_path, const char *out_path,
                                ThongTinTep *ti)
{
    ErrCode e = ttt_load(in_path, ti);

    if (e != ERR_OK) {
        return e;
    }
    if (same_path(in_path, out_path)) {
        return ERR_SAME_FILE;
    }
    return ERR_OK;
}

static ErrCode verify_header(const Header *h, uint64_t cipher_total)
{
    if (check_block_n(h->block_n) != ERR_OK) {
        return ERR_BAD_MAGIC;
    }
    if ((h->flags & FLAG_PADDED) && cipher_total == 0) {
        return ERR_BAD_CRC;
    }
    return ERR_OK;
}

ErrCode decrypt_file(const char *in_path, const char *out_path,
                     const Key *key, cipher_lookup lookup)
{
    ThongTinTep ti;
    Header h;
    DecState st = {0, 0, 0};
    FILE *fi = NULL, *fo = NULL;
    unsigned char *buf = NULL;
    cipher_fn fn;
    char tmp[600];
    ErrCode e;

    if (key == NULL || lookup == NULL) {
        return ERR_BAD_ARG;
    }
    if (key->len == 0) {
        return ERR_EMPTY_KEY;
    }
    e = open_for_decrypt(in_path, out_path, &ti);
    if (e != ERR_OK) {
        return e;
    }
    fi = fopen(in_path, "rb");
    if (fi == NULL) {
        return ERR_NO_READ;
    }
    e = header_read(fi, &h);
    st.cipher_total = ti.size >= HEADER_SIZE ? ti.size - HEADER_SIZE : 0;
    fn = (e == ERR_OK) ? lookup(h.alg) : NULL;
    if (e == ERR_OK) {
        e = (fn == NULL) ? ERR_BAD_MAGIC : verify_header(&h, st.cipher_total);
    }
    if (e != ERR_OK) {
        fclose(fi);
        return e;
    }
    snprintf(tmp, sizeof tmp, "%s.tmp", out_path);
    fo = fopen(tmp, "wb");
    buf = malloc(BLOCK_SIZE + MAX_PAD);
    if (fo == NULL || buf == NULL) {
        free(buf);
        return finish_output(fi, fo, tmp, out_path, buf == NULL ? ERR_NOMEM : ERR_IO);
    }
    e = decrypt_loop(fi, fo, &h, key, fn, buf, &st);
    if (e == ERR_OK && (st.plain_out != h.orig_size || st.crc != h.crc)) {
        e = ERR_BAD_CRC;
    }
    free(buf);
    return finish_output(fi, fo, tmp, out_path, e);
}
