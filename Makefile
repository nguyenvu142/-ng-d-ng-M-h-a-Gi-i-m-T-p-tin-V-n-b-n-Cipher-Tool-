CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -I. -Isrc -Idocs

# Tự động tìm tất cả file .c ở thư mục gốc, src/ và docs/
SRCS = $(wildcard *.c) $(wildcard src/*.c) $(wildcard docs/*.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

debug: CFLAGS += -g -fsanitize=address
debug: clean $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o src/*.o docs/*.o $(TARGET)
