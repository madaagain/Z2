

#include "game_info.h"

void send_tile(server_t *server, int y, int x)
{
    tile_t tile = server->map.tiles[y][x];

    if (server->graphical_socket == -1)
        return;
    dprintf(server->graphical_socket, "bct %d %d %d %d %d %d %d %d %d\n",
        x, y, tile.resources[0], tile.resources[1], tile.resources[2],
        tile.resources[3], tile.resources[4],
        tile.resources[5], tile.resources[6]);
}

void send_tile_content(server_t *server, char *command)
{
    char *token = strtok(command, " ");
    char *x_str = strtok(NULL, " ");
    char *y_str = strtok(NULL, " ");
    int x = 0;
    int y = 0;
    tile_t tile;

    if (server->graphical_socket == -1 || x_str == NULL || y_str == NULL)
        return;
    x = atoi(x_str);
    y = atoi(y_str);
    if (x < 0 || y < 0 || x >= server->map.width || y >= server->map.height)
        return;
    tile = server->map.tiles[y][x];
    UNUSED(token);
    dprintf(server->graphical_socket, "bct %d %d %d %d %d %d %d %d %d\n",
        x, y, tile.resources[0], tile.resources[1], tile.resources[2],
        tile.resources[3], tile.resources[4],
        tile.resources[5], tile.resources[6]);
}

void send_all_tiles_content(server_t *server, char *command)
{
    tile_t tile;

    UNUSED(command);
    if (server->graphical_socket == -1)
        return;
    for (int y = 0; y < server->map.height; y++) {
        for (int x = 0; x < server->map.width; x++) {
            tile = server->map.tiles[y][x];
            dprintf(server->graphical_socket,
                "bct %d %d %d %d %d %d %d %d %d\n",
                x, y, tile.resources[0], tile.resources[1], tile.resources[2],
                tile.resources[3], tile.resources[4],
                tile.resources[5], tile.resources[6]);
        }
    }
}
