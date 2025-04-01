/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** client.h
*/

#ifndef CLIENT_H_
    #define CLIENT_H_

typedef struct client_s {
    int client_socket;
    char *command;
} client_t;

#endif /* !CLIENT_H_ */
