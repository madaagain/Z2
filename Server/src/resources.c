/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** ressources.c
*/

#include "ia_command.h"
#include "game_info.h"

static const char *resources_names[] = {
    "food",
    "linemate",
    "deraumere",
    "sibur",
    "mendiane",
    "phiras",
    "thystame"
};

static void send_take_graphic(server_t *server, player_t *player, int resource)
{
    if (server->graphical_socket == -1)
        return;
    dprintf(server->graphical_socket, "pgt #%d %d\n", player->id, resource);
}

static void send_set_graphic(server_t *server, player_t *player, int resource)
{
    if (server->graphical_socket == -1)
        return;
    dprintf(server->graphical_socket, "pdr #%d %d\n", player->id, resource);
}

void take_object(player_t *player, server_t *server, const char *object)
{
    tile_t *tile = &server->map.tiles[player->y][player->x];

    player->cooldown = 7;
    if (object == NULL) {
        dprintf(player->socket, "ko\n");
        return;
    }
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (strcmp(object, resources_names[i]) == 0
            && tile->resources[i] > 0) {
            player->inventory[i]++;
            tile->resources[i]--;
            server->resources[i]--;
            dprintf(player->socket, "ok\n");
            send_take_graphic(server, player, i);
            return;
        }
    }
    dprintf(player->socket, "ko\n");
}

void set_object(player_t *player, server_t *server, const char *object)
{
    tile_t *tile = &server->map.tiles[player->y][player->x];

    player->cooldown = 7;
    if (object == NULL) {
        dprintf(player->socket, "ko\n");
        return;
    }
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (strcmp(object, resources_names[i]) == 0
            && player->inventory[i] > 0) {
            player->inventory[i]--;
            tile->resources[i]++;
            server->resources[i]++;
            dprintf(player->socket, "ok\n");
            send_set_graphic(server, player, i);
            return;
        }
    }
    dprintf(player->socket, "ko\n");
}

void show_inventory(player_t *player, server_t *server, const char *args)
{
    char buffer[1024] = {0};

    UNUSED(server);
    UNUSED(args);
    sprintf(buffer, "[food %d, linemate %d, deraumere %d, "
        "sibur %d, mendiane %d, phiras %d, thystame %d]\n",
        player->inventory[0], player->inventory[1], player->inventory[2],
        player->inventory[3], player->inventory[4], player->inventory[5],
        player->inventory[6]);
    dprintf(player->socket, "%s", buffer);
    player->cooldown = 1;
}
