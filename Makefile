CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -ggdb
tron : main.c
	$(CC) $(CFLAGS) -o tron main.c -lraylib
