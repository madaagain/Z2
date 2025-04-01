

#include "server.h"
#include "client.h"
#include "ia_command.h"
#include "graphic_command.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

int register_new_client(
    server_t *server, int client_socket_tmp
)
{
    FILE* sock_f = NULL;

    if (client_socket_tmp == -1) {
        fprintf(stderr, "ERROR: Server could not accept the client\n");
        return 0;
    }
    if (server->player_count < server->max_client) {
        sock_f = fdopen(client_socket_tmp, "r");
        if (sock_f == NULL)
            perror("fdopen fail");
        add_player_to_server(&(server->players),
            create_new_player(server, client_socket_tmp, sock_f));
        server->player_count++;
        dprintf(client_socket_tmp, "WELCOME\n");
    } else
        dprintf(client_socket_tmp, "ERROR: Server is full\n");
    return 0;
}

static void close_player_socket(server_t *server, player_t *player,
    FILE *sock_f)
{
    remove_player_from_tile(&server->map.tiles[player->y][player->x], player);
    remove_player(server, player);
    close(player->socket);
    fclose(sock_f);
    player->socket = -1;
    free(player);
    player = NULL;
}

static int read_client_socket(server_t *server, player_t *player)
{
    char *byte = NULL;
    size_t size_byte = 0;
    ssize_t bytes_read;

    if (player->sock_f == NULL)
        return ERROR;
    bytes_read = getline(&byte, &size_byte, player->sock_f);
    if (bytes_read <= 0) {
        send_player_death(server, player);
        close_player_socket(server, player, player->sock_f);
        free(byte);
        return 1;
    }
    byte[bytes_read] = '\0';
    add_command_to_queue(player, byte);
    return 0;
}

int check_message_client(server_t *server, player_t *player)
{
    int return_value = 0;

    if (FD_ISSET(player->socket, &server->readfd)) {
        return_value = read_client_socket(server, player);
        if (return_value == 1)
            return 1;
        if (return_value == ERROR)
            return ERROR;
    }
    if (player->cooldown == 0 && player->in_incantation == 0
        && handle_client_command(server, player,
        pop_command_from_queue(player)) == 1)
        return 1;
    return 0;
}

void run_frame(server_t *server, player_t *player)
{
    if (player->team_id == DEFAULT_TEAM_ID)
        return;
    if (player->cooldown > 0)
        player->cooldown--;
    if (player->food_cooldown > 0) {
        player->food_cooldown--;
    } else {
        if (player->inventory[FOOD] == 0) {
            send_player_death(server, player);
            close_player_socket(server, player, player->sock_f);
            return;
        }
        player->inventory[FOOD]--;
        player->food_cooldown = 126;
    }
    if (player->cooldown == 0 && player->in_incantation != 0 &&
        player->incantation_players != NULL) {
        verif_incantation_requirements(player, server);
        player->in_incantation = 0;
    }
}

static int run_client(server_t *server, players_list_t **tmp)
{
    players_list_t *next = (*tmp)->next;
    int return_value = check_message_client(server, (*tmp)->value);

    if (return_value == ERROR)
        return ERROR;
    if (return_value == 1) {
        *tmp = next;
        return 1;
    }
    if ((*tmp)->value->team_id == DEFAULT_TEAM_ID)
        return 0;
    return 0;
}

int read_the_client(server_t *server, arguments_t *arg)
{
    players_list_t *tmp = server->players;
    int return_value = 0;

    UNUSED(arg);
    while (tmp != NULL) {
        return_value = run_client(server, &tmp);
        if (return_value == 1)
            return 0;
        if (return_value == ERROR)
            return ERROR;
        tmp = tmp->next;
    }
    if (server->graphical_socket != -1
        && parsing_message_graphic(server) == ERROR)
        return ERROR;
    return 0;
}

static void write_in_all_client(server_t *server, char *buf)
{
    players_list_t *tmp = server->players;

    while (tmp != NULL) {
        dprintf(tmp->value->socket, "%s", buf);
        tmp = tmp->next;
    }
    if (server->graphical_socket != -1) {
        dprintf(server->graphical_socket, "%s", buf);
    }
}

static int manage_accept(
    server_t *server, int cli_socket_tmp, arguments_t *arg)
{
    char *buffer = NULL;
    size_t size_buffer = 0;

    if (FD_ISSET(server->socket, &server->readfd)) {
        cli_socket_tmp =
            accept(server->socket, (struct sockaddr *)&server->info_serv,
                                                    &server->struc_sock_len);
        register_new_client(server, cli_socket_tmp);
    }
    if (FD_ISSET(0, &server->readfd)) {
        if (getline(&buffer, &size_buffer, stdin) == -1) {
            free(buffer);
            return BREAK;
        } else {
            write_in_all_client(server, buffer);
            free(buffer);
        }
    }
    return read_the_client(server, arg);
}

int manage_select(
    server_t *server, int cli_socket_tmp, arguments_t *arg)
{
    struct timeval timeout;

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    if (select(FD_SETSIZE, &server->readfd, NULL, NULL, &timeout) < 0) {
        free_all_server(server);
        return ERROR;
    }
    return manage_accept(server, cli_socket_tmp, arg);
}
