/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** graphic_command.c
*/

#include "ia_command.h"
#include "game_info.h"
#include "graphic_command.h"
#include <stdio.h>

static const graphic_command_t GRAPHIC_COMMANDS[] = {
    {"msz", (void (*)(server_t *, char *)) &send_map_size},
    {"bct", (void (*)(server_t *, char *)) &send_tile_content},
    {"mct", (void (*)(server_t *, char *)) &send_all_tiles_content},
    {"tna", (void (*)(server_t *, char *)) &send_all_teams},
    {"ppo", (void (*)(server_t *, char *)) &send_player_position},
    {"plv", (void (*)(server_t *, char *)) &send_player_level},
    {"pin", (void (*)(server_t *, char *)) &send_player_inventory},
    {"sgt", (void (*)(server_t *, char *)) &send_time_unit},
    {"sst", (void (*)(server_t *, char *)) &update_time_unit},
    {NULL, NULL}
};

void send_player_level(server_t *server, char *command)
{
    char *token = strtok(command, " ");
    char *player_id_str = strtok(NULL, " ");
    int player_id = 0;
    players_list_t *tmp = server->players;

    if (server->graphical_socket == -1 || player_id_str == NULL)
        return;
    player_id = atoi(player_id_str);
    UNUSED(token);
    while (tmp != NULL) {
        if (tmp->value->id == player_id) {
            dprintf(server->graphical_socket, "plv #%d %d\n",
                tmp->value->id, tmp->value->level);
            return;
        }
        tmp = tmp->next;
    }
}

static void write_inventory_in_socket(server_t *server, players_list_t *tmp)
{
    dprintf(server->graphical_socket,
        "pin #%d %d %d %d %d %d %d %d %d %d\n",
        tmp->value->id, tmp->value->x, tmp->value->y,
        tmp->value->inventory[0], tmp->value->inventory[1],
        tmp->value->inventory[2], tmp->value->inventory[3],
        tmp->value->inventory[4], tmp->value->inventory[5],
        tmp->value->inventory[6]);
}

void send_player_inventory(server_t *server, char *command)
{
    char *token = strtok(command, " ");
    char *player_id_str = strtok(NULL, " ");
    int player_id = 0;
    players_list_t *tmp = server->players;

    if (server->graphical_socket == -1 || player_id_str == NULL)
        return;
    player_id = atoi(player_id_str);
    UNUSED(token);
    while (tmp != NULL) {
        if (tmp->value->id == player_id) {
            write_inventory_in_socket(server, tmp);
            return;
        }
        tmp = tmp->next;
    }
}

void send_time_unit(server_t *server, char *command)
{
    if (server->graphical_socket == -1)
        return;
    UNUSED(command);
    dprintf(server->graphical_socket, "sgt %d\n", server->freq);
}

void send_player_death(server_t *server, player_t *player)
{
    dprintf(player->socket, "dead\n");
    if (server->graphical_socket == -1)
        return;
    dprintf(server->graphical_socket, "pdi #%d\n", player->id);
}

static void handle_graphic_command(server_t *server, char *command)
{
    if (server->graphical_socket == -1)
        return;
    for (int i = 0; GRAPHIC_COMMANDS[i].command_name != NULL; i++) {
        if (strncmp(GRAPHIC_COMMANDS[i].command_name, command, 3) == 0) {
            GRAPHIC_COMMANDS[i].command_function(server, command);
            return;
        }
    }
    fprintf(stderr, "Unknown graphic command: %s\n", command);
}

static void formate_byte_str(char *byte)
{
    byte[0] = byte[1];
    for (int i = 1; byte[i] != '\0'; i++) {
        byte[i] = byte[i + 1];
    }
}

int parsing_message_graphic(server_t *server)
{
    char *byte = NULL;
    size_t size_byte = 0;
    ssize_t bytes_read;

    if (FD_ISSET(server->graphical_socket, &server->readfd)) {
        bytes_read = getline(&byte, &size_byte, server->graphical_sock_f);
        if (bytes_read <= 0) {
            close(server->graphical_socket);
            free(byte);
            server->graphical_socket = -1;
            return 0;
        }
        byte[bytes_read] = '\0';
        if (byte[0] == '\0' && bytes_read > 1) {
            formate_byte_str(byte);
        }
        handle_graphic_command(server, byte);
        free(byte);
    }
    return 0;
}
