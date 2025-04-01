
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

int verif_incantation(
    tile_t *tile, player_t *player, server_t *server, int level
)
{
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (tile->resources[i] < incantation_requirements[level][i]) {
            send_end_incantation(server, player, 0);
            failed_incatation(player);
            return 0;
        }
    }
    return 1;
}
