

#include "game_info.h"
#include "ia_command.h"

static const int incantation_requirements[7][RESOURCE_COUNT] = {
    {0, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 0, 0},
    {0, 2, 0, 1, 0, 2, 0},
    {0, 1, 1, 2, 0, 1, 0},
    {0, 1, 2, 1, 3, 0, 0},
    {0, 1, 2, 3, 0, 1, 0},
    {0, 2, 2, 2, 2, 2, 1}
};

static const int player_needed[7] = {1, 2, 2, 4, 4, 6, 6};

static void check_equal_level(players_list_t **tmp, player_t *player,
    int *player_count, player_t **players)
{
    if ((*tmp)->value->level == player->level &&
        (*tmp)->value->in_incantation == 0) {
        players[*player_count] = (*tmp)->value;
        (*player_count)++;
    }
    (*tmp) = (*tmp)->next;
}

player_t **check_nb_players_requirements(player_t *player, server_t *server,
    int player_needed)
{
    tile_t *tile = &server->map.tiles[player->y][player->x];
    players_list_t *tmp = tile->players;
    int player_count = 1;
    player_t **players = malloc(sizeof(player_t *) * player_needed);

    players[0] = player;
    while (tmp) {
        if (player_count >= player_needed)
            break;
        if (tmp->value->id == player->id) {
            tmp = tmp->next;
            continue;
        }
        check_equal_level(&tmp, player, &player_count, players);
    }
    if (player_count < player_needed) {
        free(players);
        return NULL;
    }
    return players;
}

static void level_up_players(server_t *server, player_t *player, int level)
{
    for (int i = 0; i < player_needed[level]; i++) {
        player->incantation_players[i]->in_incantation = 0;
        player->incantation_players[i]->level++;
        dprintf(player->incantation_players[i]->socket, "Current level: %d\n",
        player->incantation_players[i]->level);
        if (server->graphical_socket != -1)
            dprintf(server->graphical_socket, "plv #%d %d\n",
                player->incantation_players[i]->id,
                player->incantation_players[i]->level);
    }
    if (player->incantation_players[0]->level == 8) {
        if (server->graphical_socket != -1) {
            dprintf(server->graphical_socket, "seg %s\n",
                server->teams[player->team_id].name);
            fprintf(stderr, "Team %s won the game !\n",
                server->teams[player->team_id].name);
        }
        server->end_of_game = 1;
    }
}

void send_end_incantation(server_t *server, player_t *player, int success)
{
    if (server->graphical_socket == -1)
        return;
    dprintf(server->graphical_socket, "pie %d %d %d\n", player->x, player->y,
        success);
}

static void failed_incatation(player_t *player)
{
    if (player->incantation_players == NULL)
        return;
    for (int i = 0; i < player_needed[player->level - 1]; i++) {
        player->incantation_players[i]->in_incantation = 0;
        dprintf(player->incantation_players[i]->socket, "ko\n");
    }
    free(player->incantation_players);
    player->incantation_players = NULL;
}

static int all_players_on_tile(server_t *server, player_t *player)
{
    int all_on_same_tile = 1;

    if (player->incantation_players == NULL)
        return 0;
    for (int i = 0; i < player_needed[player->level - 1]; i++) {
        if (player->incantation_players[i]->x != player->x ||
            player->incantation_players[i]->y != player->y) {
            all_on_same_tile = 0;
            break;
        }
    }
    if (all_on_same_tile == 0) {
        failed_incatation(player);
        send_end_incantation(server, player, 0);
        return 0;
    }
    send_end_incantation(server, player, 1);
    return 1;
}

int verif_incantation_requirements(player_t *player, server_t *server)
{
    tile_t *tile = &server->map.tiles[player->y][player->x];
    int level = player->level - 1;

    if (player == NULL || player->incantation_players == NULL)
        return 0;
    if (verif_incantation(tile, player, server, level) == 0)
        return 0;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        tile->resources[i] -= incantation_requirements[level][i];
        server->resources[i] -= incantation_requirements[level][i];
    }
    if (all_players_on_tile(server, player) == 0)
        return 0;
    level_up_players(server, player, level);
    free(player->incantation_players);
    player->incantation_players = NULL;
    return 1;
}

static int check_available_resources(server_t *server, player_t *player,
    player_t **players)
{
    int level = player->level - 1;

    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (server->map.tiles[player->y][player->x].resources[i] <
            incantation_requirements[level][i]) {
            dprintf(player->socket, "ko\n");
            free(players);
            return 1;
        }
    }
    return 0;
}

void send_start_incantation(server_t *server, player_t *player)
{
    if (server->graphical_socket == -1)
        return;
    dprintf(server->graphical_socket, "pic %d %d %d", player->x, player->y,
        player->level);
    for (int i = 0; i < player_needed[player->level - 1]; i++) {
        dprintf(server->graphical_socket, " #%d",
            player->incantation_players[i]->id);
    }
    dprintf(server->graphical_socket, "\n");
}

void start_incantation(player_t *player, server_t *server, const char *args)
{
    int level = player->level - 1;
    player_t **players = check_nb_players_requirements(player,
        server, player_needed[level]);

    UNUSED(args);
    if (!players) {
        dprintf(player->socket, "ko\n");
        return;
    }
    player->incantation_players = players;
    if (check_available_resources(server, player, players) == 1) {
        player->incantation_players = NULL;
        return;
    }
    for (int i = 0; i < player_needed[level]; i++) {
        players[i]->in_incantation = 1;
        players[i]->cooldown = 300;
        dprintf(players[i]->socket, "Elevation underway\n");
    }
    send_start_incantation(server, player);
}
