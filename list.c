#include "app.h"

// Return items in incoming buffer/list that 
// are ready for processing
int count_incoming_list() {
    int length = 0;
    while (incoming_list[length][0] != 0) {
        length++;
    }
    return length;
}

// Return items in outgoing buffer/list that
// are ready for processing
int count_outgoing_list() {
    int length = 0;
    while (outgoing_list[length][0] != 0) {
        length++;
    }
    return length;
}

// Configure incoming buffer/list/array used
// to store incoming strings from UART
void init_incoming_list() {
    for (int i = 0; i < MAX_INCOMING_LIST_SIZE; i++) {
        strcpy(incoming_list[i], "");
    }
}

// Configure outgoing buffer/list/array used
// to store outgoing strings to be sent to UART
void init_outgoing_list() {
    for (int i = 0; i < MAX_OUTGOING_LIST_SIZE; i++) {
        strcpy(outgoing_list[i], "");
    }
}

// Adds new item to incoming list
int add_to_incoming(const char *word) {
    int ret = 1;
    if (word != NULL) {
        if (strlen(word) >= MAX_STRING_LENGTH) {
            ret = 0;
        } else {
            if (count_incoming_list() >= MAX_INCOMING_LIST_SIZE) {
				printf("Too many incoming commands - skipping\n");
                ret = 1;
            } else {
                for (int i = MAX_INCOMING_LIST_SIZE - 1; i > 0; i--) {
                    strcpy(incoming_list[i], incoming_list[i - 1]);
                }
                strcpy(incoming_list[0], word);
            }
        }
    }
    return ret;
}

// Adds new item to outcoming list
int add_to_outgoing(const char *word) {
    int ret = 1;
    if (word != NULL) {
        if (strlen(word) >= MAX_STRING_LENGTH) {
            ret = 0;
        } else {
            if (count_outgoing_list() >= MAX_OUTGOING_LIST_SIZE) {
				printf("Too many incoming commands - skipping\n");
                ret = 1;
            } else {
                for (int i = MAX_OUTGOING_LIST_SIZE - 1; i > 0; i--) {
                    strcpy(outgoing_list[i], outgoing_list[i - 1]);
                }
                strcpy(outgoing_list[0], word);
            }
        }
    }
    return ret;
}

// Retrieve the next item from the incoming list,
// delete it from the list, and return it as a string
char *get_next_incoming() {
    static char message[MAX_STRING_LENGTH];
    if (count_incoming_list() == 0) {
        return NULL;
    }

    memset(message, 0, sizeof(message));

    for (int i = MAX_INCOMING_LIST_SIZE - 1; i >= 0; i--) {
        if (incoming_list[i][0] != 0) {
            strcpy(message, incoming_list[i]);
            strcpy(incoming_list[i], "");
            break;
        }
    }
    return message;
}

// Retrieve the next item from the outgoing list,
// delete it from the list, and return it as a string
char *get_next_outgoing() {
    static char message[MAX_STRING_LENGTH];
    if (count_outgoing_list() == 0) {
        return NULL;
    }

    memset(message, 0, sizeof(message));

    for (int i = MAX_OUTGOING_LIST_SIZE - 1; i >= 0; i--) {
        if (outgoing_list[i][0] != 0) {
            strcpy(message, outgoing_list[i]);
            strcpy(outgoing_list[i], "");
            break;
        }
    }
    return message;
}
