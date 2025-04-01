
#ifndef CLIENT_H_
    #define CLIENT_H_

typedef struct client_s {
    int client_socket;
    char *command;
} client_t;

#endif /* !CLIENT_H_ */
