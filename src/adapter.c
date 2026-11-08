# Nếu tệp padding.c đang nằm ở tests/ hoặc ngoài gốc, đưa về src/
[ -f tests/padding.c ] && cp tests/padding.c src/
[ -f padding.c ] && mv padding.c src/

# Đảm bảo các tệp cài đặt XOR và Transpose cũng nằm trong src/
# (Ví dụ: xorcipher.c, transpose.c, permutation_module.c)