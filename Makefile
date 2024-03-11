.phony: build run clean

CC=gcc
CFLAGS=-Wall -Werror -g
TARGET=tema3

build: $(TARGET)

$(TARGET): tema3.c tema3.h
	$(CC) $(CFLAGS) -o $(TARGET) tema3.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
