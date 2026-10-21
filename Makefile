CC = gcc
CFLAGS = -Wall -Wextra
TARGET = build/space-traveler

all:
	mkdir -p build
	$(CC) $(CFLAGS) src/main.c -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -rf build