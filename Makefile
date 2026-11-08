CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -O2 -Isrc
DBGFLAGS = -std=c99 -Wall -Wextra -pedantic -g -O0 -fsanitize=address,undefined -Isrc

SRCS = $(wildcard src/*.c)
TARGET = cipher_tool

.PHONY: all debug test clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

debug: $(SRCS)
	$(CC) $(DBGFLAGS) $(SRCS) -o $(TARGET)_debug

test: all debug
	bash tests/run_tests.sh

clean:
	rm -f $(TARGET) $(TARGET)_debug *.tmp tests/*.bin tests/*.enc tests/*.dec
