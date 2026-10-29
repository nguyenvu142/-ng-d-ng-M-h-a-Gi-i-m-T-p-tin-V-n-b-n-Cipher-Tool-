1.	Kiểm tra chất lượng
gcc -Wall -Wextra -std=c11 -pedantic -o gen_data tests/gen_data.c gcc -Wall -Wextra -std=c11 -pedantic -o run_tests tests/run_tests.c ./gen_data ./run_tests


Làm được ngay bằng gcc trên Windows. Chạy từng file với cờ của đề, ghi cảnh báo vào tệp:



1.	gcc -Wall -Wextra -std=c11 -pedantic docs\main.c docs\tables.c docs\caesar.c 
2.	docs\vigenere.c docs\validate.c -o cipher 2> canhbao.txt



ASan/UBSan, Valgrind, cppcheck cần WSL (đề ghi “qua WSL”). Cài một lần: mở PowerShell bằng quyền Administrator, gõ wsl --install, khởi động lại máy, tạo tài khoản Ubuntu. Trong Ubuntu gõ:
1.	sudo apt update
2.	sudo apt install -y build-essential valgrind cppcheck
3.	cd "/mnt/c/Users/NHUT MINH/Desktop/<thư mục repo>"
4.	mkdir -p data out
Rồi chạy:
1.	gcc -g -fsanitize=address,undefined -std=c11 docs/*.c -o cipher_asan
2.	gcc -std=c11 -DCIPHER_CMD='"./cipher_asan"' tests/run_tests.c -o run_tests_asan
3.	gcc tests/gen_data.c -o gen_data && ./gen_data
4.	./run_tests_asan

5.	gcc -g -std=c11 docs/*.c -o cipher
6.	printf '1\n1\n1\ndata/8192bit.bin\nout/v.tmp\n3\n0\n' > ans.tmp
7.	valgrind --leak-check=full ./cipher < ans.tmp
8.	cppcheck --enable=all --std=c11 docs/ 2> cppcheck.txt

2. Demo tấn công
Tôi viết và chạy thử hai chương trình, nằm ở file đính kèm. Chép vào tests\.
Bẻ Caesar (attack_caesar.c): thử mọi khóa rồi chấm điểm bản giải mã (nhiều ký tự in được và nhiều chữ hay gặp là đúng). Hỗ trợ cả chế độ byte (255 khóa, mặc định của nhóm) và chế độ chữ cái (25 khóa).
1.	gcc -Wall -Wextra -std=c11 -pedantic tests\attack_caesar.c -o attack_caesar
2.	attack_caesar
Nó hỏi tên tệp đã mã hóa và chế độ.

Bẻ XOR bằng known-plaintext (attack_xor.c): vì bản mã = bản rõ XOR khóa, nên khóa = bản mã XOR bản rõ. Chương trình hỏi tệp chứa đoạn bản rõ kẻ tấn công biết (ví dụ phần đầu Dear customer,) và tệp bản mã, rồi tìm khóa, chu kỳ lặp, và giải mã cả tệp. Tôi thử với khóa KEY: ra đúng khóa và tệp giải mã giống hệt bản gốc.
