#include "caesar.h"
#include "tables.h"

int caesar_process(
    unsigned char *buf,
    size_t n,
    int key,
    size_t offset,
    int op,
    int mode
)
{
    size_t i;
    int shift;

    (void)offset;

    if (buf == NULL && n > 0) {
        return -1;
    }

    if (key < 1 || key > 255) {
        return -1;
    }

    if (op != OP_ENCRYPT && op != OP_DECRYPT) {
        return -1;
    }

    if (mode != MODE_BYTE && mode != MODE_ALPHA) {
        return -1;
    }

    shift = key;

    if (op == OP_DECRYPT) {
        shift = -shift;
    }

    if (mode == MODE_BYTE) {
        build_caesar_tables(key);

        for (i = 0; i < n; i++) {
            if (op == OP_ENCRYPT) {
                buf[i] = caesar_table[buf[i]];
            } else {
                buf[i] = caesar_inverse_table[buf[i]];
            }
        }
    } else {
        shift = ((shift % 26) + 26) % 26;

        for (i = 0; i < n; i++) {
            if (buf[i] >= 'A' && buf[i] <= 'Z') {
                buf[i] = (unsigned char)(
                    'A' + (buf[i] - 'A' + shift) % 26
                );
            } else if (buf[i] >= 'a' && buf[i] <= 'z') {
                buf[i] = (unsigned char)(
                    'a' + (buf[i] - 'a' + shift) % 26
                );
            }
        }
    }

    return 0;
}
