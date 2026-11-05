CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-parameter -pedantic -std=c99 -I. -Isrc -Idocs
TARGET = cipher_tool

# Tự động lấy các file .c thực sự tồn tại trong src/ (và docs/ nếu có, trừ các file chứa hàm main phụ)
SRCS = $(wildcard src/*.c) $(filter-out docs/main.c docs/xorcipher.c docs/test_classic.c docs/reference_check.c, $(wildcard docs/*.c))
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

debug: CFLAGS += -g -fsanitize=address
debug: clean $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o docs/*.o *.o $(TARGET)
