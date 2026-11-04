# CRC32 module (Huy)

## Convention used
This implementation uses the standard CRC-32/ISO-HDLC (also commonly called
CRC-32/ADCCP) parameters:

- Width: 32 bits
- Reflected polynomial: `0xEDB88320` (normal polynomial `0x04C11DB7`)
- Initial register: `0xFFFFFFFF`
- Reflected input/output
- Final XOR: `0xFFFFFFFF`
- Standard check value: ASCII `123456789` -> `0xCBF43926`

The task sheet entry only identifies `crc32.c`; it does not specify a CRC variant.
This module therefore documents the conventional CRC-32/ISO-HDLC choice explicitly.
Confirm this convention with the group specification before integrating if it names
a different polynomial or initialization/finalization rule.

## API
- `crc32_init`: initialize a streaming context.
- `crc32_update`: append bytes; may be called repeatedly for file chunks.
- `crc32_finalize`: obtain the final 32-bit checksum.
- `crc32_compute`: one-call convenience function.

`crc32_update` accepts `NULL` only when `length == 0`. CRC32 is an error-detection
checksum, not encryption, authentication, or a cryptographic hash.

## Build and test (from the project root)
```sh
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror crc32.c tests/test_crc32.c -o test_crc32
./test_crc32
```
On Windows, run `test_crc32.exe`.

Expected output:
`CRC32 tests passed (4/4).`
