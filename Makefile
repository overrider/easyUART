CFLAGS = -g -Wall -std=gnu99
CC = gcc

all:
	$(CC) $(CFLAGS) main_signal.c list.c functions.c -o uart_app

clean:
	$(RM) *.o uart_app
