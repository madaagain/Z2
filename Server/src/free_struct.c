/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** free_struct.c
*/

#include "game_info.h"
#include "server.h"

static void set_previous(server_t *server, players_list_t *current,
    players_list_t *prev)
{
    if (prev == NULL) {
        server->players = current->next;
    } else {
        prev->next = current->next;
    }
}

static void free_player_queue(queue_t *queue)
{
    queue_t *tmp = queue;
    queue_t *to_free = NULL;

    while (tmp) {
        free(tmp->command);
        to_free = tmp;
        tmp = tmp->next;
        free(to_free);
    }
}

static void remove_incantation_player(player_t *player)
{
    if (player != NULL && player->incantation_players != NULL) {
        free(player->incantation_players);
        player->incantation_players = NULL;
    }
}

static void check_is_egg(server_t *server, player_t *player)
{
    server->player_count--;
    if (player->team_id == DEFAULT_TEAM_ID)
        return;
    if (player->is_egg) {
        server->teams[player->team_id].max_players--;
        server->teams[player->team_id].player_count--;
    } else {
        server->teams[player->team_id].player_count--;
        server->teams[player->team_id].available_slots++;
    }
}

void remove_player(server_t *server, player_t *player)
{
    players_list_t *current = server->players;
    players_list_t *prev = NULL;

    while (current != NULL) {
        if (current->value->id == player->id) {
            set_previous(server, current, prev);
            check_is_egg(server, player);
            free_player_queue(player->command_queue);
            remove_incantation_player(player);
            free(current);
            break;
        }
        prev = current;
        current = current->next;
    }
}

void free_all_players(players_list_t *players)
{
    players_list_t *current = players;
    players_list_t *next = NULL;

    while (current != NULL) {
        next = current->next;
        fclose(current->value->sock_f);
        free(current->value);
        free(current);
        current = next;
    }
}

void free_tile(tile_t *tile)
{
    players_list_t *current = tile->players;
    players_list_t *next = NULL;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

void free_all_tiles(tile_t **tiles, int width, int height)
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            free_tile(&tiles[i][j]);
        }
        free(tiles[i]);
    }
}

static void free_eggs(server_t *server)
{
    egg_queue_t *current = server->eggs;
    egg_queue_t *next = NULL;

    while (current != NULL) {
        next = current->next;
        free(current->egg);
        free(current);
        current = next;
    }
}

void free_all_server(server_t *server)
{
    free(server->teams);
    free_all_players(server->players);
    free_all_tiles(server->map.tiles, server->map.width, server->map.height);
    free(server->map.tiles);
    free_eggs(server);
    if (server->graphical_socket != -1)
        close(server->graphical_socket);
    if (server->graphical_sock_f != NULL)
        fclose(server->graphical_sock_f);
}
