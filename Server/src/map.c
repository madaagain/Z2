/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** map.c
*/

#include "game_info.h"
#include "ia_command.h"
#include "graphic_command.h"
#include <time.h>

static const float resources_density[] = {
    0.5,
    0.3,
    0.15,
    0.1,
    0.1,
    0.08,
    0.05
};

int check_if_resources(tile_t *tile, int k, int *l, server_t *server)
{
    if (tile->resources[k] < 1) {
        tile->resources[k] += 1;
        *l += 1;
        server->resources[k] += 1;
        return 1;
    }
    return 0;
}

static void place_resource_on_tile(server_t *server, int k, int *l)
{
    int i = rand() % server->map.height;
    int j = rand() % server->map.width;

    if (check_if_resources(&server->map.tiles[i][j], k, l, server) == 1)
        send_tile(server, i, j);
}

void generate_resources(server_t *server)
{
    int nb_resources = 0;
    int l = 0;

    srand(time(NULL));
    for (int k = 0; k < RESOURCE_COUNT; k++) {
        nb_resources = server->map.width *
            server->map.height * resources_density[k];
        if (nb_resources < 1)
            nb_resources = 1;
        nb_resources -= server->resources[k];
        while (l < nb_resources) {
            place_resource_on_tile(server, k, &l);
        }
        l = 0;
    }
}

static void set_map_variable(server_t *server, int width, int height)
{
    server->map.width = width;
    server->map.height = height;
    server->map.tiles = malloc(sizeof(tile_t *) * height);
}

void init_map(server_t *server, int width, int height)
{
    set_map_variable(server, width, height);
    if (server->map.tiles == NULL) {
        fprintf(stderr, "ERROR: malloc failed\n");
        return;
    }
    for (int i = 0; i < height; i++) {
        server->map.tiles[i] = malloc(sizeof(tile_t) * width);
        if (server->map.tiles[i] == NULL) {
            fprintf(stderr, "ERROR: malloc failed\n");
            return;
        }
        for (int j = 0; j < width; j++) {
            server->map.tiles[i][j].player_count = 0;
            server->map.tiles[i][j].players = NULL;
            memset(server->map.tiles[i][j].resources,
                0, sizeof(int) * RESOURCE_COUNT);
            server->map.tiles[i][j].nb_egg = 0;
        }
    }
}
