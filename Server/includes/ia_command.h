

#ifndef IA_COMMAND_H
    #define IA_COMMAND_H

    #include "game_info.h"

void move_player_forward(player_t *player, server_t *server, const char *args);
void turn_player_right(player_t *player, server_t *server, const char *args);
void turn_player_left(player_t *player, server_t *server, const char *args);
void look_around(player_t *player, server_t *server, const char *args);
void show_inventory(player_t *player, server_t *server, const char *args);
void broadcast_text(player_t *player, server_t *server, const char *text);
void connect_nbr(player_t *player, server_t *server, const char *args);
void fork_player(player_t *player, server_t *server, const char *args);
void eject_players(player_t *player, server_t *server, const char *args);
void take_object(player_t *player, server_t *server, const char *object);
void set_object(player_t *player, server_t *server, const char *object);
void start_incantation(player_t *player, server_t *server, const char *object);

void remove_player_from_tile(tile_t *tile, player_t *player);
void add_player_to_tile(tile_t *tile, player_t *player);
int handle_client_command(server_t *server, player_t *player,
    char *command);

void start_incantation(player_t *player, server_t *server, const char *args);

typedef struct {
    const char *command_name;
    void (*command_function)(player_t *, server_t *, const char *);
} command_mapping_t;

typedef void (*incantation_function_t)(player_t *, server_t *);

#endif // IA_COMMAND_H
