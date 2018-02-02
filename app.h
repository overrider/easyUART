#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <errno.h>
#include <termios.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define DEBUG 1

#define UART_DEVICE "/dev/pts/3"
#define BAUD_RATE B19200

#define INPUT_BUFFER_SIZE 1024
#define MAX_STRING_LENGTH 1024
#define MAX_OUTGOING_LIST_SIZE 512
#define MAX_INCOMING_LIST_SIZE 512

extern char incoming_list[MAX_INCOMING_LIST_SIZE][MAX_STRING_LENGTH];
extern char outgoing_list[MAX_OUTGOING_LIST_SIZE][MAX_STRING_LENGTH];

extern unsigned long current_time;
extern unsigned long last_pok;
extern unsigned long time_since_last_pok;

extern int uart_status;

void init_incoming_list();
void init_outgoing_list();
int count_incoming_list();
int count_outgoing_list();
int add_to_incoming(const char *word);
int add_to_outgoing(const char *word);
char *get_next_incoming();
char *get_next_outgoing();

int is_device_ready();

int process_command(char *command);
