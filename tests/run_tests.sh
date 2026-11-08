#!/usr/bin/env bash
set -e
make clean && make all && make debug
python3 tests/gen_data.py
python3 tests/ref.py
echo "[PASS] Tat ca cac bai test da hoan tat!"
