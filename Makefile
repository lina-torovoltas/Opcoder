CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11
LDFLAGS = -lcapstone

SRC = $(wildcard src/*.c) src/capture.s

TARGET = build/opcoder

all:
	@mkdir -p build >/dev/null 2>&1
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	@rm -rf build

.PHONY: all clean