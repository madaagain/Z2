/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** look.c
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

void is_first_resources(char *resources, int *first)
{
    if (!*first) {
        strcat(resources, " ");
    } else {
        *first = 0;
    }
}

static void add_egg_to_str(char *resources, int *first, int nb_egg)
{
    for (int i = 0; i < nb_egg; i++) {
        is_first_resources(resources, first);
        strcat(resources, "egg");
    }
}

char *tile_resources_to_str(tile_t *tile)
{
    char *resources = malloc(sizeof(char) * 1024);
    int first = 1;
    players_list_t *current = tile->players;

    if (resources == NULL)
        return NULL;
    resources[0] = '\0';
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        for (int j = 0; j < tile->resources[i]; j++) {
            is_first_resources(resources, &first);
            strcat(resources, resources_names[i]);
        }
    }
    while (current != NULL) {
        is_first_resources(resources, &first);
        strcat(resources, "player");
        current = current->next;
    }
    add_egg_to_str(resources, &first, tile->nb_egg);
    return resources;
}

void is_first_tile_on_line(char *reponse, int j, int i)
{
    if (j != -i) {
        strcat(reponse, ",");
    }
}

static int get_x_value(player_t *player, int i, int j, server_t *server)
{
    if (player->direction == NORTH)
        return (player->x + j + server->map.height) % server->map.width;
    if (player->direction == SOUTH)
        return (player->x - j + server->map.width) % server->map.width;
    if (player->direction == EAST)
        return (player->x + i + server->map.width) % server->map.width;
    return (player->x - i + server->map.height) % server->map.width;
}

static int get_y_value(player_t *player, int i, int j, server_t *server)
{
    if (player->direction == NORTH)
        return (player->y - i + server->map.height) % server->map.height;
    if (player->direction == SOUTH)
        return (player->y + i + server->map.height) % server->map.height;
    if (player->direction == EAST)
        return (player->y + j + server->map.height) % server->map.height;
    return (player->y - j + server->map.height) % server->map.height;
}

void look_around(player_t *player, server_t *server, const char *args)
{
    int x = player->x;
    int y = player->y;
    char reponse[4096] = "[";
    char *tile_resources;

    UNUSED(args);
    for (int i = 0; i <= player->level; i++) {
        for (int j = -i; j <= i; j++) {
            is_first_tile_on_line(reponse, j, i);
            x = get_x_value(player, i, j, server);
            y = get_y_value(player, i, j, server);
            tile_resources = tile_resources_to_str(&server->map.tiles[y][x]);
            strcat(reponse, tile_resources);
            free(tile_resources);
        }
        if (i != player->level)
            strcat(reponse, ",");
    }
    dprintf(player->socket, "%s\n", strcat(reponse, "]"));
    player->cooldown = 7;
}
