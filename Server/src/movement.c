

#include "ia_command.h"
#include "graphic_command.h"

static int remove_from_id(players_list_t *prev, players_list_t *current,
    player_t *player, tile_t *tile)
{
    if (current->value->id == player->id) {
        if (prev == NULL)
            tile->players = current->next;
        else
            prev->next = current->next;
        free(current);
        tile->player_count--;
        return 1;
    }
    return 0;
}

void remove_player_from_tile(tile_t *tile, player_t *player)
{
    players_list_t *current = tile->players;
    players_list_t *prev = NULL;

    while (current != NULL) {
        if (remove_from_id(prev, current, player, tile) == 1)
            return;
        prev = current;
        current = current->next;
    }
}

void add_player_to_tile(tile_t *tile, player_t *player)
{
    players_list_t *new = malloc(sizeof(players_list_t));
    players_list_t *current = tile->players;

    if (new == NULL) {
        fprintf(stderr, "ERROR: malloc failed\n");
        return;
    }
    new->value = player;
    new->next = NULL;
    if (current == NULL) {
        tile->players = new;
    } else {
        while (current->next != NULL)
            current = current->next;
        current->next = new;
    }
    tile->player_count++;
}

void move_player_forward(player_t *player, server_t *server, const char *args)
{
    int x = player->x;
    int y = player->y;

    UNUSED(args);
    if (player->direction == NORTH)
        y = (y - 1 + server->map.height) % server->map.height;
    if (player->direction == SOUTH)
        y = (y + 1) % server->map.height;
    if (player->direction == WEST)
        x = (x - 1 + server->map.width) % server->map.width;
    if (player->direction == EAST)
        x = (x + 1) % server->map.width;
    remove_player_from_tile(&(server->map.tiles[player->y][player->x]),
        player);
    add_player_to_tile(&(server->map.tiles[y][x]), player);
    player->x = x;
    player->y = y;
    dprintf(player->socket, "ok\n");
    send_player_position_from_player(player, server);
    player->cooldown = 7;
}

void turn_player_right(player_t *player, server_t *server, const char *args)
{
    UNUSED(args);
    UNUSED(server);
    player->direction = player->direction + 1;
    if (player->direction > 4)
        player->direction = 1;
    dprintf(player->socket, "ok\n");
    send_player_position_from_player(player, server);
    player->cooldown = 7;
}

void turn_player_left(player_t *player, server_t *server, const char *args)
{
    UNUSED(args);
    UNUSED(server);
    player->direction = player->direction - 1;
    if (player->direction < 1)
        player->direction = 4;
    dprintf(player->socket, "ok\n");
    send_player_position_from_player(player, server);
    player->cooldown = 7;
}
