#include "app.h"

// Check if the UART_DEVICE exists and can be accessed
int is_device_ready() {
	if (access(UART_DEVICE, F_OK) != -1) {
        return 1;
    } else {
        return 0;
    }
}

// Pass a command and perform actions as needed
int process_command(char *command){
	if (strcmp(command, "#POK") == 0) {
		last_pok = time(NULL);
		uart_status = 1;
	} else if(strcmp(command, "EXIT") == 0){
		//do_exit = 1;
	} else {
		printf("Unhandled command: %s\n", command);
		fflush(NULL);
	}
	return 1;
}
