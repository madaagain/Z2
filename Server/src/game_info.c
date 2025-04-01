
#include "game_info.h"
#include "ia_command.h"
#include "graphic_command.h"
#include <time.h>
#include <stdlib.h>

static void add_additional_default_info(player_t *new)
{
    new->cooldown = 0;
    new->team_id = DEFAULT_TEAM_ID;
    new->queue_size = 0;
    new->command_queue = NULL;
    new->in_incantation = 0;
    new->food_cooldown = 126;
    new->incantation_players = NULL;
    new->is_egg = 0;
}

player_t *create_new_player(server_t *server, int socket, FILE *sock_f)
{
    player_t *new = malloc(sizeof(player_t));

    if (new == NULL) {
        fprintf(stderr, "ERROR: malloc failed\n");
        return NULL;
    }
    srand(time(NULL));
    new->id = server->last_id;
    server->last_id++;
    new->x = rand() % server->map.width;
    new->y = rand() % server->map.height;
    new->direction = rand() % 4 + 1;
    new->level = DEFAULT_LEVEL;
    memset(new->inventory, 0, sizeof(int) * RESOURCE_COUNT);
    new->inventory[FOOD] = DEFAULT_LIFE_UNITS;
    new->socket = socket;
    new->sock_f = sock_f;
    add_additional_default_info(new);
    return new;
}

void add_player_to_server(players_list_t **players, player_t *player)
{
    players_list_t *new = malloc(sizeof(players_list_t));
    players_list_t *current = *players;

    if (new == NULL)
        return;
    new->value = player;
    new->next = NULL;
    if (current == NULL) {
        *players = new;
    } else {
        while (current->next != NULL)
            current = current->next;
        current->next = new;
    }
}

void init_server(server_t *server, arguments_t *args)
{
    server->port = args->port;
    server->freq = args->freq;
    server->last_id = 0;
    server->team_count = count_elements(args->name);
    server->teams = NULL;
    server->players = NULL;
    server->player_count = 0;
    server->graphical_socket = -1;
    server->eggs = NULL;
    server->graphical_sock_f = NULL;
    server->respawn_cooldown = DEFAULT_RESPAWN_COOLDOWN;
    server->frame_elapsed_ns = 1.0 / server->freq * 1000000;
    server->end_of_game = 0;
    memset(server->resources, 0, sizeof(int) * RESOURCE_COUNT);
    init_map(server, args->width, args->height);
    generate_resources(server);
    send_all_tiles_content(server, NULL);
    create_teams(server, args->name, args->clientsNb);
}

void create_teams(server_t *server, char **teams, int max_players)
{
    server->teams = malloc(sizeof(team_t) * server->team_count);
    if (server->teams == NULL) {
        fprintf(stderr, "ERROR: malloc failed\n");
        return;
    }
    for (int i = 0; i < server->team_count; i++) {
        strcpy(server->teams[i].name, teams[i]);
        server->teams[i].player_count = 0;
        server->teams[i].max_players = max_players;
        server->teams[i].available_slots = max_players;
    }
}

void update_time_unit(server_t *server, char *command)
{
    char *token = strtok(command, " ");
    char *time_unit_str = strtok(NULL, " ");
    int time_unit = 0;

    if (server->graphical_socket == -1 || time_unit_str == NULL)
        return;
    time_unit = atoi(time_unit_str);
    UNUSED(token);
    server->freq = time_unit;
    dprintf(server->graphical_socket, "sst %d\n", time_unit);
}
