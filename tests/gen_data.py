import os
os.makedirs("tests/data", exist_ok=True)
for name, sz in [("empty_0B.bin", 0), ("one_1B.bin", 1), ("block_exact_8B.bin", 8), ("chunk_64KB.bin", 65536)]:
    with open(os.path.join("tests/data", name), "wb") as f:
        f.write(os.urandom(sz))
