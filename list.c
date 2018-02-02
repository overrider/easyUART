/*
 * =====================================================================================
 *
 *       Filename:  list.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/31/2018 05:54:27 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "app.h"

int count_incoming_list()
{
    int length = 0;
    while (incoming_list[length][0] != 0) {
        length++;
    }
    return length;
}

int count_outgoing_list() {
    int length = 0;
    while (outgoing_list[length][0] != 0) {
        length++;
    }
    return length;
}

void init_incoming_list() {
    for (int i = 0; i < MAX_INCOMING_LIST_SIZE; i++)
    {
        strcpy(incoming_list[i], "");
    }
}

void init_outgoing_list() {
    for (int i = 0; i < MAX_OUTGOING_LIST_SIZE; i++) {
        strcpy(outgoing_list[i], "");
    }
}

// adds new item to incoming list
int add_to_incoming(const char *word) {
    int ret = 1;
    if (word != NULL) {
        if (strlen(word) >= MAX_STRING_LENGTH) {
            ret = 0;
        } else {
            if (count_incoming_list() >= MAX_INCOMING_LIST_SIZE) {
				printf("Too many incoming commands - skipping\n");
                ret = 1;
            }
            else
            {
                for (int i = MAX_INCOMING_LIST_SIZE - 1; i > 0; i--)
                {
                    strcpy(incoming_list[i], incoming_list[i - 1]);
                }
                strcpy(incoming_list[0], word);
            }
        }
    }
    return ret;
}

// adds new item to outcoming list
int add_to_outgoing(const char *word)
{
    int ret = 1;
    if (word != NULL)
    {
        if (strlen(word) >= MAX_STRING_LENGTH)
        {
            ret = 0;
        }
        else
        {
            if (count_outgoing_list() >= MAX_OUTGOING_LIST_SIZE)
            {
				printf("Too many incoming commands - skipping\n");
                ret = 1;
            }
            else
            {
                for (int i = MAX_OUTGOING_LIST_SIZE - 1; i > 0; i--)
                {
                    strcpy(outgoing_list[i], outgoing_list[i - 1]);
                }
                strcpy(outgoing_list[0], word);
            }
        }
    }
    return ret;
}


char *get_next_incoming()
{
    static char message[MAX_STRING_LENGTH];
    if (count_incoming_list() == 0) {
        return NULL;
    }

    memset(message, 0, sizeof(message));

    // 1.) get next item from list
    // 2.) delete item from list
    // 3.) return item as string
    for (int i = MAX_INCOMING_LIST_SIZE - 1; i >= 0; i--) {
        if (incoming_list[i][0] != 0) {
            strcpy(message, incoming_list[i]);
            strcpy(incoming_list[i], "");
            break;
        }
    }
    return message;
}

char *get_next_outgoing()
{
    static char message[MAX_STRING_LENGTH];
    if (count_outgoing_list() == 0) {
        return NULL;
    }

    memset(message, 0, sizeof(message));

    // 1.) get next item from list
    // 2.) delete item from list
    // 3.) return item as string
    for (int i = MAX_OUTGOING_LIST_SIZE - 1; i >= 0; i--) {
        if (outgoing_list[i][0] != 0) {
            strcpy(message, outgoing_list[i]);
            strcpy(outgoing_list[i], "");
            break;
        }
    }
    return message;
}
