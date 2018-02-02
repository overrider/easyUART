#include "app.h"

void signal_handler_IO(int status);

int n;
int serial_fd;
int connected;
struct termios term_attr;
struct sigaction saio;

static volatile int do_exit = 0;

// Input buffer used for receiving UART characters
char input_buffer[INPUT_BUFFER_SIZE];
int input_buffer_index = 0;

// Represents a queue/list for strings received/send
char incoming_list[MAX_INCOMING_LIST_SIZE][MAX_STRING_LENGTH];
char outgoing_list[MAX_OUTGOING_LIST_SIZE][MAX_STRING_LENGTH];

// Variables used to determine UART status.
// Basically, this code sends #PNG every 10 seconds
// to the UART. It expects the UART device to reply
// with #POK. If no #POK is received, we can assume
// there is a communication issue with the device.
unsigned long current_time = 0;
unsigned long last_pok 	   = 0;
unsigned long time_since_last_pok = 0;

int uart_status = 1;

// open connection to uart
int uart_open() {
    int fd;
    fd = open(UART_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("Unable to open serial port\n");
        return -1;
    }

    saio.sa_handler = signal_handler_IO;
    saio.sa_flags = 0;
    saio.sa_restorer = NULL;
    sigaction(SIGIO,&saio,NULL);

    fcntl(fd, F_SETOWN, getpid());
    fcntl(fd, F_SETFL, FNDELAY|FASYNC);

    tcgetattr(fd, &term_attr);
    cfsetispeed(&term_attr, BAUD_RATE);
    cfsetospeed(&term_attr, BAUD_RATE);
    term_attr.c_cflag &= ~PARENB;
    term_attr.c_cflag &= ~CSTOPB;
    term_attr.c_cflag &= ~CSIZE;
    term_attr.c_cflag |= CS8;
    term_attr.c_cflag |= (CLOCAL | CREAD);
    term_attr.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    term_attr.c_iflag &= ~(IXON | IXOFF | IXANY);
    term_attr.c_oflag &= ~OPOST;
    tcsetattr(fd, TCSANOW, &term_attr);
	if(DEBUG){
		printf("UART1 (%s) configured....\n", UART_DEVICE);
	}
    return fd;
}

// close the connection
int uart_close() {
    close(serial_fd);
    return 1;
}

void signal_handler_IO (int status) {
    unsigned char c;
    int readCount;

    do {
        readCount = read(serial_fd, &c, 1);
        if (readCount < 0) {
            break;
        } else if (readCount > 0) {
            if (isalnum(c) || c == '#') {
                input_buffer[input_buffer_index++] = c;
            } else if ((c == '\n') || (c == '\r')) {
                if (strlen(input_buffer) > 0) {
                    add_to_incoming(input_buffer);
                    memset(input_buffer, 0, sizeof(input_buffer));
                    input_buffer_index = 0;
                }
            }
         }
    } while (readCount != 0);
}

void int_handler(int param) {
    do_exit = 1;
	signal(SIGINT, int_handler);
}

int main(int argc, char *argv[]) {    
    do_exit = 0;

    signal(SIGIO, SIG_IGN);
    signal(SIGINT, int_handler);

    init_incoming_list();
    init_outgoing_list();

    while(is_device_ready() == 0) {
        printf("Waiting for %s to be plugged in\n", UART_DEVICE);

        if (do_exit != 0) {
            exit(do_exit);
        }
        sleep(1);
    }

    serial_fd = uart_open();
    if (serial_fd < 0) {
        printf("Failed to open serial port!\n");
        exit(1);
    }
    printf("Opening UART....OK\n");

	last_pok = time(NULL);

    while (do_exit == 0) {
		// Create a timeout for 1 second
		struct timespec timeout;
		timeout.tv_sec = 1;
		timeout.tv_nsec = 0;
		while (nanosleep(&timeout, &timeout) && errno == EINTR);

		current_time = time(NULL);
		time_since_last_pok = current_time - last_pok;

		if (time_since_last_pok >= 30){
			uart_status = 0;
		}

		if(current_time % 10 == 0){
			add_to_outgoing("#PNG\r");
		}

		if (count_incoming_list() > 0) {
			char *command = get_next_incoming();
			if (command != NULL) {
				process_command(command);
			}
		}

		if (count_outgoing_list() > 0) {
			char *command = get_next_outgoing(); // returns and removes item from the outgoing list
			if (command != NULL) {
				int sent_count = write(serial_fd, command, strlen(command));

				if (sent_count != (strlen(command))) {
					printf("Sorry couldn't send. Maybe device have issue\n");
				}
			}
		}

		if(DEBUG){
			printf("INCOMING: %d OUTGOING: %d UART: %d\n",
					count_incoming_list(),
					count_outgoing_list(),
					uart_status
			);
		}
	}
    uart_close();
    printf("exit reason: %d\n", do_exit);
    exit(0);
}
