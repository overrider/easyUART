/*
 * =====================================================================================
 *
 *       Filename:  functions.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/31/2018 06:14:04 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "app.h"

// check if the UART_DEVICE exists, meaning is plugged in
// maybe some other fancy methods to check?
int is_device_ready() {
	if (access(UART_DEVICE, F_OK) != -1) {
        return 1;
    } else {
        return 0;
    }
}

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
