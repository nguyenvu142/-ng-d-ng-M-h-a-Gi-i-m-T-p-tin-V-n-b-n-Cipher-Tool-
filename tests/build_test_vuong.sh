#!/bin/sh
# Chay tu thu muc goc cua repo:  sh tests/build_test_vuong.sh
set -e
F="-std=c11 -O2 -Wall -Wextra -Isrc -Idocs -Itests"
for f in docs/xorcipher.c tests/transpose_process.c tests/padding.c tests/permutation_module.c; do
  b=$(basename $f .c)
  gcc $F -Dmain=${b}_demo -c $f -o /tmp/$b.o 2>/dev/null || gcc $F -Dmain=${b}_demo -c $f -o $b.o
done
gcc $F tests/test_vuong.c src/common.c src/fileinfo.c src/header.c src/fileio.c src/adapter.c \
    docs/caesar.c docs/vigenere.c docs/tables.c \
    /tmp/xorcipher.o /tmp/transpose_process.o /tmp/padding.o /tmp/permutation_module.o -o test_vuong
echo "Build xong. Chay: ./test_vuong   hoac   ./test_vuong big"
