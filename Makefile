CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -I. -Isrc -Idocs

# Liệt kê rõ ràng các tệp nguồn chính của nhóm ở src/ và docs/
SRCS = src/main.c src/header.c src/keyutil.c src/stub_identity.c \
       docs/caesar.c docs/vigenere.c docs/xorcipher.c docs/tables.c docs/validate.c

OBJS = $(SRCS:.c=.o)
TARGET = cipher_tool

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

debug: CFLAGS += -g -fsanitize=address
debug: clean $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o docs/*.o *.o $(TARGET)
