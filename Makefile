CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
SRC_DIR = src
TARGET = cipher_tool

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/ui.c $(SRC_DIR)/fileinfo.c $(SRC_DIR)/header.c $(SRC_DIR)/process_file.c $(SRC_DIR)/tables.c $(SRC_DIR)/caesar.c $(SRC_DIR)/vigenere.c $(SRC_DIR)/xorcipher.c $(SRC_DIR)/keyutil.c $(SRC_DIR)/crc32.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

debug: CFLAGS += -g -fsanitize=address
debug: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)
