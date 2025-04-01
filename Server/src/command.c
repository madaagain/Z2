/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** command.c
*/

#include "game_info.h"
#include "ia_command.h"

void connect_nbr(player_t *player, server_t *server, const char *args)
{
    int team_id = player->team_id;
    int unused_slots = server->teams[team_id].available_slots;

    UNUSED(args);
    dprintf(player->socket, "%d\n", unused_slots);
}

void fork_player(player_t *player, server_t *server, const char *args)
{
    add_egg(server, player);
    server->max_client++;
    server->teams[player->team_id].available_slots++;
    server->teams[player->team_id].max_players++;
    UNUSED(args);
    dprintf(player->socket, "ok\n");
    player->cooldown = 42;
}

static void eject_player(player_t *player, server_t *server,
    player_t *current_player)
{
    if (player->direction == NORTH) {
        current_player->y = (current_player->y - 1) % server->map.height;
        dprintf(current_player->socket, "eject: %d\n", 1);
    }
    if (player->direction == SOUTH) {
        current_player->y = (current_player->y + 1) % server->map.height;
        dprintf(current_player->socket, "eject: %d\n", 3);
    }
    if (player->direction == EAST) {
        current_player->x = (current_player->x + 1) % server->map.width;
        dprintf(current_player->socket, "eject: %d\n", 2);
    }
    if (player->direction == WEST) {
        current_player->x = (current_player->x - 1) % server->map.width;
        dprintf(current_player->socket, "eject: %d\n", 4);
    }
    remove_player_from_tile(&server->map.tiles[player->y][player->x],
        current_player);
    add_player_to_tile(&server->map.tiles[current_player->y][current_player->x]
        , current_player);
}

static void destroy_egg(server_t *server, player_t *player)
{
    egg_queue_t *tmp = server->eggs;
    egg_queue_t *tmp2 = NULL;

    while (tmp != NULL) {
        if (tmp->egg->x == player->x && tmp->egg->y == player->y) {
            tmp2 = tmp->next;
            death_of_egg(server, tmp);
            tmp = tmp2;
            continue;
        }
        tmp = tmp->next;
    }
}

void eject_players(player_t *player, server_t *server, const char *args)
{
    tile_t *tile = &server->map.tiles[player->y][player->x];
    players_list_t *current = tile->players;
    player_t *current_player = current->value;

    if (server->graphical_socket != -1) {
        dprintf(server->graphical_socket, "pex #%d\n", player->id);
    }
    for (int i = 0; i < tile->player_count; i++) {
        current_player = current->value;
        if (current->value->id != player->id) {
            eject_player(player, server, current_player);
        }
        current = current->next;
    }
    destroy_egg(server, player);
    UNUSED(args);
    player->cooldown = 7;
    dprintf(player->socket, "ok\n");
}
