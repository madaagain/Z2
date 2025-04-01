/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** graphic_command.h
*/

#ifndef GRAPHIC_COMMAND_H
    #define GRAPHIC_COMMAND_H

    #include "game_info.h"

void send_map_size(server_t *server, char *command);
void send_tile_content(server_t *server, char *command);
void send_all_tiles_content(server_t *server, char *command);
void send_all_teams(server_t *server, char *command);
void send_player_position(server_t *server, char *command);
void send_tile(server_t *server, int y, int x);
int parsing_message_graphic(server_t *server);
void send_new_player_position(player_t *player, server_t *server);
void send_player_level(server_t *server, char *command);
void send_player_inventory(server_t *server, char *command);
void send_time_unit(server_t *server, char *command);
void send_player_position_from_player(player_t *player, server_t *server);
void send_player_death(server_t *server, player_t *player);
void update_time_unit(server_t *server, char *command);

typedef struct {
    const char *command_name;
    void (*command_function)(server_t *, char *);
} graphic_command_t;

#endif // GRAPHIC_COMMAND_H
