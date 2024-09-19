CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = echo_server

$(TARGET): echo_server.c echo_server.h
	$(CC) $(CFLAGS) -o $(TARGET) echo_server.c

clean:
	rm -f $(TARGET)