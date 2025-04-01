/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** send_graphic.c
*/

#include "graphic_command.h"

void send_map_size(server_t *server, char *command)
{
    UNUSED(command);
    if (server->graphical_socket == -1)
        return;
    dprintf(server->graphical_socket, "msz %d %d\n",
        server->map.width, server->map.height);
}

void send_all_teams(server_t *server, char *command)
{
    UNUSED(command);
    if (server->graphical_socket == -1)
        return;
    for (int i = 0; i < server->team_count; i++) {
        dprintf(server->graphical_socket, "tna %s\n", server->teams[i].name);
    }
}

void send_new_player_position(player_t *player, server_t *server)
{
    if (server->graphical_socket == -1)
        return;
    dprintf(server->graphical_socket, "pnw #%d %d %d %d %d %s\n",
        player->id, player->x, player->y, player->direction,
        player->level, server->teams[player->team_id].name);
}

void send_player_position_from_player(player_t *player, server_t *server)
{
    if (server->graphical_socket == -1)
        return;
    dprintf(server->graphical_socket, "ppo #%d %d %d %d\n",
        player->id, player->x, player->y, player->direction);
}

void send_player_position(server_t *server, char *command)
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
            dprintf(server->graphical_socket, "ppo #%d %d %d %d\n",
                tmp->value->id, tmp->value->x, tmp->value->y,
                tmp->value->direction);
            return;
        }
        tmp = tmp->next;
    }
}
