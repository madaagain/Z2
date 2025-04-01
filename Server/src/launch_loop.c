

#include "server.h"
#include "client.h"
#include "game_info.h"
#include "graphic_command.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

static long time_diff_ns(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) * 1000000 +
        (end->tv_usec - start->tv_usec);
}

void set_all_clients_socket(server_t *server)
{
    players_list_t *tmp = server->players;

    while (tmp != NULL) {
        if (tmp->value->socket > 0)
            FD_SET(tmp->value->socket, &server->readfd);
        tmp = tmp->next;
    }
    if (server->graphical_socket > 0)
        FD_SET(server->graphical_socket, &server->readfd);
}

static void run_server_frame(server_t *server, struct timeval *start,
    struct timeval *end)
{
    players_list_t *tmp = server->players;
    players_list_t *tmp_next = NULL;

    gettimeofday(end, NULL);
    server->frame_elapsed_ns -= time_diff_ns(start, end);
    if (server->frame_elapsed_ns > 0)
        return;
    server->frame_elapsed_ns = 1.0 / server->freq * 1000000;
    if (server->respawn_cooldown > 0) {
        server->respawn_cooldown -= 1;
    } else {
        server->respawn_cooldown = DEFAULT_RESPAWN_COOLDOWN;
        generate_resources(server);
    }
    while (tmp != NULL) {
        tmp_next = tmp->next;
        run_frame(server, tmp->value);
        tmp = tmp_next;
    }
}

static void set_server_socket(server_t *server, struct timeval *start)
{
    gettimeofday(start, NULL);
    FD_ZERO(&server->readfd);
    FD_SET(server->socket, &server->readfd);
    FD_SET(0, &server->readfd);
    set_all_clients_socket(server);
}

int connection_loop(server_t *server, arguments_t *arg)
{
    int client_socket_tmp = 0;
    int check = 0;
    struct timeval start;
    struct timeval end;

    server->struc_sock_len = sizeof(*(&server->info_serv));
    while (!global_catch(0)) {
        set_server_socket(server, &start);
        check = manage_select(server, client_socket_tmp, arg);
        if (check == ERROR)
            return ERROR;
        if (check == BREAK)
            break;
        run_server_frame(server, &start, &end);
        if (server->end_of_game == 1)
            break;
    }
    free_all_server(server);
    return 0;
}

int initialize_client_set(server_t *server, arguments_t *arg)
{
    listen(server->socket, 1);
    return connection_loop(server, arg);
}

static int set_socket(server_t *server, arguments_t *arg, int opt)
{
    server->info_serv.sin_family = AF_INET;
    server->info_serv.sin_port = htons(arg->port);
    server->info_serv.sin_addr.s_addr = INADDR_ANY;
    if (setsockopt(server->socket, SOL_SOCKET, SO_REUSEADDR,
                                                    &opt, sizeof(opt)) != 0) {
        fprintf(stderr, "ERROR: setsockopt fail\n");
        return ERROR;
    }
    if (bind(server->socket, (struct sockaddr *)&server->info_serv,
                                            sizeof(server->info_serv)) == -1) {
        fprintf(stderr, "ERROR: Bind fail\n");
        return ERROR;
    }
    return 0;
}

int created_socket(arguments_t *arg)
{
    int opt = 1;
    server_t server;

    server.socket = socket(AF_INET, SOCK_STREAM, 0);
    fprintf(stderr, "server socket = %d\n", server.socket);
    if (server.socket == -1) {
        fprintf(stderr, "ERROR: creation socket fail\n");
        return ERROR;
    }
    if (set_socket(&server, arg, opt) == ERROR)
        return ERROR;
    server.max_client = count_elements(arg->name) * arg->clientsNb;
    fprintf(stderr, "maximum clients on the server = %d\n", server.max_client);
    fprintf(stderr, "===========================================\n");
    init_server(&server, arg);
    return initialize_client_set(&server, arg);
}
