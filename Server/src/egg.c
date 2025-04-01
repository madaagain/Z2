

#include "server.h"
#include "game_info.h"

static void send_new_egg_graphic(server_t *server, egg_t *egg,
    player_t *player)
{
    if (server->graphical_socket != -1) {
        dprintf(server->graphical_socket, "enw #%d #%d %d %d\n", egg->id,
            player->id, egg->x, egg->y);
        dprintf(server->graphical_socket, "pfk #%d\n", player->id);
    }
}

static void set_new_egg_variable(server_t *server, player_t *player,
    egg_t *egg)
{
    egg->id = server->last_id;
    server->last_id++;
    egg->x = player->x;
    egg->y = player->y;
    egg->team_id = player->team_id;
    egg->team_name = server->teams[player->team_id].name;
    send_new_egg_graphic(server, egg, player);
}

void add_egg(server_t *server, player_t *player)
{
    egg_queue_t *new_egg = malloc(sizeof(egg_queue_t));
    egg_queue_t *tmp = server->eggs;

    if (new_egg == NULL)
        return;
    new_egg->next = NULL;
    new_egg->egg = malloc(sizeof(egg_t));
    if (new_egg->egg == NULL)
        return;
    set_new_egg_variable(server, player, new_egg->egg);
    server->map.tiles[player->y][player->x].nb_egg++;
    if (tmp == NULL) {
        server->eggs = new_egg;
        return;
    }
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = new_egg;
}

void remove_egg(server_t *server, egg_t *egg)
{
    egg_queue_t *tmp = server->eggs;
    egg_queue_t *prev = NULL;

    if (tmp->egg->id == egg->id) {
        server->eggs = tmp->next;
        free(tmp->egg);
        free(tmp);
        return;
    }
    while (tmp != NULL) {
        if (tmp->egg->id == egg->id) {
            prev->next = tmp->next;
            server->map.tiles[tmp->egg->y][tmp->egg->x].nb_egg--;
            free(tmp->egg);
            free(tmp);
            return;
        }
        prev = tmp;
        tmp = tmp->next;
    }
}

void death_of_egg(server_t *server, egg_queue_t *egg)
{
    if (server->graphical_socket != -1)
        dprintf(server->graphical_socket, "edi %d\n", egg->egg->id);
    server->teams[egg->egg->team_id].available_slots--;
    server->teams[egg->egg->team_id].max_players--;
    server->map.tiles[egg->egg->y][egg->egg->x].nb_egg--;
    remove_egg(server, egg->egg);
}
