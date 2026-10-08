#ifndef MESSAGES_H
#define MESSAGES_H

enum {
    MESSAGE_COUNT = 5,
    MESSAGE_SIZE = 80,
};

extern char MESSAGES[MESSAGE_COUNT][MESSAGE_SIZE];
extern int MESSAGE_INDEX;

void message_add(const char *message);

#endif
