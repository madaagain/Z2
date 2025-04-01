/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** server.h
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #define ERROR (84)
    #define BREAK (18)
    #define HELP (18)

    #include <sys/select.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <getopt.h>
    #include "client.h"
    #include "game_info.h"

typedef struct {
    char opt;
    int (*handler)(arguments_t *arg, char *optarg, char **av, int ac);
} option_handler_t;

/* MAIN_C */
void dash_help(int ac, char **av);
int parse_arg(int ac, char **av, arguments_t *arg);
int created_socket(arguments_t *arg);
void print_info(arguments_t arg);
int global_catch(int _catch);
/* !MAIN_C */

/* LAUNCH_LOOP.C */
int manage_select(
    server_t *server, int client_socket_tmp, arguments_t *arg
);
/* LAUNCH_LOOP.C */

/* PARSE_ARG.C */
void print_help(void);
int handle_port(arguments_t *arg, char *optarg, char **av, int ac);
int handle_width(arguments_t *arg, char *optarg, char **av, int ac);
int handle_height(arguments_t *arg, char *optarg, char **av, int ac);
int handle_name(arguments_t *arg, char *optarg, char **av, int ac);
int handle_clients_nb(arguments_t *arg, char *optarg, char **av, int ac);
/* !PARSE_ARG.C */

/* PRINT_INFO.C */
int count_elements(char **array);
/* !PRINT_INFO.C */

#endif /* !SERVER_H_ */
