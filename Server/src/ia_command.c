/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** in_the_loop.c
*/

#include "game_info.h"
#include "ia_command.h"
#include "graphic_command.h"

static const command_mapping_t command_map[] = {
    {"Forward",
        (void (*)(player_t *, server_t *, const char *))move_player_forward},
    {"Right",
        (void (*)(player_t *, server_t *, const char *))turn_player_right},
    {"Left",
        (void (*)(player_t *, server_t *, const char *))turn_player_left},
    {"Look", (void (*)(player_t *, server_t *, const char *))look_around},
    {"Inventory",
        (void (*)(player_t *, server_t *, const char *))show_inventory},
    {"Broadcast",
        (void (*)(player_t *, server_t *, const char *))broadcast_text},
    {"Connect_nbr",
        (void (*)(player_t *, server_t *, const char *))connect_nbr},
    {"Fork", (void (*)(player_t *, server_t *, const char *))fork_player},
    {"Eject", (void (*)(player_t *, server_t *, const char *))eject_players},
    {"Take", (void (*)(player_t *, server_t *, const char *))take_object},
    {"Set", (void (*)(player_t *, server_t *, const char *))set_object},
    {"Incantation",
        (void (*)(player_t *, server_t *, const char *))start_incantation},
    {NULL, NULL}
};

int get_team_id_from_name(server_t *server, const char *team_name)
{
    for (int i = 0; i < server->team_count; i++) {
        if (strcmp(server->teams[i].name, team_name) == 0) {
            return i;
        }
    }
    return -1;
}

int add_player_to_team(server_t *server, player_t *player,
    const char *team_name)
{
    int team_id = get_team_id_from_name(server, team_name);

    if (team_id == -1) {
        return -1;
    }
    if (server->teams[team_id].available_slots <= 0) {
        return -1;
    }
    player->team_id = team_id;
    server->teams[team_id].available_slots--;
    server->teams[team_id].player_count++;
    return 0;
}

static void send_connection_of_egg(server_t *server, egg_t *egg)
{
    if (server->graphical_socket != -1) {
        dprintf(server->graphical_socket, "ebo #%d\n", egg->id);
    }
}

static void check_egg_in_team(server_t *server, player_t *player)
{
    egg_queue_t *tmp = server->eggs;

    while (tmp != NULL) {
        if (tmp->egg->team_id == player->team_id) {
            player->y = tmp->egg->y;
            player->x = tmp->egg->x;
            player->is_egg = true;
            server->map.tiles[tmp->egg->y][tmp->egg->x].nb_egg--;
            send_connection_of_egg(server, tmp->egg);
            remove_egg(server, tmp->egg);
            return;
        }
        tmp = tmp->next;
    }
}

static int is_valid_team(server_t *server, player_t *player,
    const char *command)
{
    if (add_player_to_team(server, player, command) == 0) {
        dprintf(player->socket, "%d\n",
            server->teams[player->team_id].available_slots);
        dprintf(player->socket, "%d %d\n",
            server->map.width, server->map.height);
        check_egg_in_team(server, player);
        add_player_to_tile(&server->map.tiles[player->y][player->x], player);
        send_new_player_position(player, server);
        return 1;
    }
    return 0;
}

static int run_command(server_t *server, player_t *player,
    const char *command, const char *args)
{
    if (player->team_id == DEFAULT_TEAM_ID)
        return -1;
    for (int i = 0; command_map[i].command_name != NULL; i++) {
        if (strcmp(command_map[i].command_name, command) == 0) {
            command_map[i].command_function(player, server, args);
            return 1;
        }
    }
    UNUSED(args);
    return 0;
}

static void send_all_players(server_t *server)
{
    players_list_t *current = server->players;

    if (current != NULL)
        send_map_size(server, NULL);
    while (current != NULL) {
        send_new_player_position(current->value, server);
        current = current->next;
    }
}

static void handle_graphic_client(server_t *server, player_t *player)
{
    server->graphical_socket = player->socket;
    remove_player_from_tile(&(server->map.tiles[player->y][player->x]),
        player);
    if (server->graphical_sock_f != NULL)
        fclose(server->graphical_sock_f);
    server->graphical_sock_f = player->sock_f;
    remove_player(server, player);
    free(player);
    send_all_players(server);
}

static int execute_command(server_t *server, player_t *player,
    char *cmd_name, char *args)
{
    int return_value = 0;

    return_value = run_command(server, player, cmd_name, args);
    if (return_value == 1 || is_valid_team(server, player, cmd_name) == 1)
        return 0;
    if (strcmp(cmd_name, "GRAPHIC") == 0) {
        handle_graphic_client(server, player);
        return 1;
    }
    if (return_value == -1)
        return 1;
    fprintf(stderr, "Unknown command: %s\n", cmd_name);
    return 0;
}

int handle_client_command(server_t *server, player_t *player,
    char *command)
{
    char cmd[256];
    char *cmd_name = NULL;
    char *args = NULL;

    if (command == NULL || strlen(command) <= 1)
        return 0;
    strcpy(cmd, command);
    free(command);
    cmd[strlen(cmd) - 1] = '\0';
    cmd_name = strtok(cmd, " ");
    args = strtok(NULL, "");
    return execute_command(server, player, cmd_name, args);
}
