# GHI CHÚ THIẾT KẾ: CAESAR, VIGENÈRE & CRC32 (HUY)
- Caesar: Bảng tra 256 byte `enc_table[i] = (i + shift) & 0xFF`.
- Vigenère: Dịch chuyển theo khóa lặp vòng `(offset + i) % L`.
