/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** game_info.h
*/

#ifndef GAME_INFO_H
    #define GAME_INFO_H

    #include <unistd.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <getopt.h>
    #include <stdbool.h>

    #define MAX_NAME_LEN 32
    #define DEFAULT_LEVEL 1
    #define DEFAULT_TEAM_ID -1
    #define DEFAULT_LIFE_UNITS 10
    #define DEFAULT_RESPAWN_COOLDOWN 20
    #define UNUSED(x) (void)(x)

    #define MAX_NAME_LEN 32

typedef enum {
    FOOD,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    RESOURCE_COUNT
} ResourceType_t;

typedef enum {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
} Direction_t;

typedef struct arguments {
    int port;
    int width;
    int height;
    char **name;
    int clientsNb;
    int freq;
    int name_count;
} arguments_t;

typedef struct queue {
    char *command;
    struct queue *next;
} queue_t;

typedef struct player {
    int id;
    int x;
    int y;
    Direction_t direction;
    int level;
    int inventory[RESOURCE_COUNT];
    int team_id;
    int socket;
    FILE *sock_f;
    int cooldown;
    int food_cooldown;
    queue_t *command_queue;
    int queue_size;
    int in_incantation;
    struct player **incantation_players;
    bool is_egg;
} player_t;

typedef struct players {
    struct player *value;
    struct players *next;
} players_list_t;

typedef struct tile {
    int resources[RESOURCE_COUNT];
    int player_count;
    int nb_egg;
    players_list_t *players;
} tile_t;

typedef struct team {
    char name[MAX_NAME_LEN];
    int player_count;
    int max_players;
    int available_slots;
} team_t;

typedef struct map {
    int width;
    int height;
    tile_t **tiles;
} map_t;

typedef struct egg {
    int id;
    int x;
    int y;
    int team_id;
    char *team_name;
} egg_t;

typedef struct egg_queue {
    struct egg *egg;
    struct egg_queue *next;
} egg_queue_t;

typedef struct server_s {
    int socket;
    fd_set readfd;
    unsigned int struc_sock_len;
    struct sockaddr_in info_serv;
    int max_client;
    int port;
    int freq;
    map_t map;
    players_list_t *players;
    team_t *teams;
    int team_count;
    int player_count;
    int last_id;
    int server_socket;
    int graphical_socket;
    FILE *graphical_sock_f;
    int resources[RESOURCE_COUNT];
    int respawn_cooldown;
    egg_queue_t *eggs;
    long frame_elapsed_ns;
    int end_of_game;
} server_t;

void init_map(server_t *server, int width, int height);
void init_server(server_t *server, arguments_t *args);
void create_teams(server_t *server, char **teams, int max_players);
void remove_player(server_t *server, player_t *player);
void generate_resources(server_t *server);
int verif_incantation_requirements(player_t *player, server_t *server);

player_t *create_new_player(server_t *server, int socket, FILE* sock_f);
void add_player_to_server(players_list_t **players, player_t *player);
int get_team_id_from_name(server_t *server, const char *team_name);
void run_frame(server_t *server, player_t *player);

int count_elements(char **array);
void free_all_server(server_t *server);

void add_command_to_queue(player_t *player, char *command);
char *pop_command_from_queue(player_t *player);

void add_egg(server_t *server, player_t *player);
void remove_egg(server_t *server, egg_t *egg);
void death_of_egg(server_t *server, egg_queue_t *egg);

int verif_incantation(
    tile_t *tile, player_t *player, server_t *server, int level
);
void send_end_incantation(server_t *server, player_t *player, int success);

#endif // GAME_INFO_H
