CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-parameter -pedantic -std=c99 -I. -Isrc -Idocs
TARGET = cipher_tool
SRCS = $(wildcard src/main.c src/header.c src/keyutil.c docs/caesar.c docs/vigenere.c docs/xorcipher.c)
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
