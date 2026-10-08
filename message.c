#include <string.h>

#include "message.h"

char MESSAGES[MESSAGE_COUNT][MESSAGE_SIZE];
int MESSAGE_INDEX = 0;

void message_add(const char *message) {
    strncpy(MESSAGES[MESSAGE_INDEX], message, MESSAGE_SIZE);
    MESSAGES[MESSAGE_INDEX][MESSAGE_SIZE - 1] = '\0';
    MESSAGE_INDEX++;
    MESSAGE_INDEX %= MESSAGE_COUNT;
}
