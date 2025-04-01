

#include "server.h"
#include "ia_command.h"
#include "args.h"
#include <math.h>

double calcule_distance(int x1, int y1, int x2, int y2)
{
    double dist;

    dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return dist;
}

static int find_direction_2(position_t *p, int point)
{
    if (calcule_distance(p->x1, p->y1, p->x2 - 1, p->y2) < p->distance) {
        point = 5;
        p->distance = calcule_distance(p->x1, p->y1, p->x2 - 1, p->y2);
    }
    if (calcule_distance(p->x1, p->y1, p->x2 - 1, p->y2 - 1) < p->distance) {
        point = 6;
        p->distance = calcule_distance(p->x1, p->y1, p->x2 - 1, p->y2 - 1);
    }
    if (calcule_distance(p->x1, p->y1, p->x2, p->y2 - 1) < p->distance) {
        point = 7;
        p->distance = calcule_distance(p->x1, p->y1, p->x2, p->y2 - 1);
    }
    if (calcule_distance(p->x1, p->y1, p->x2 + 1, p->y2 - 1) < p->distance) {
        point = 8;
        p->distance = calcule_distance(p->x1, p->y1, p->x2 + 1, p->y2 - 1);
    }
    return point;
}

int find_direction(position_t *p)
{
    int point = 0;

    p->distance = calcule_distance(p->x1, p->y1, p->x2 + 1, p->y2);
    point = 1;
    if (p->x1 == p->x2 && p->y1 == p->y2)
        return 0;
    if (calcule_distance(p->x1, p->y1, p->x2 + 1, p->y2 + 1) < p->distance) {
        point = 2;
        p->distance = calcule_distance(p->x1, p->y1, p->x2 + 1, p->y2 + 1);
    }
    if (calcule_distance(p->x1, p->y1, p->x2, p->y2 + 1) < p->distance) {
        point = 3;
        p->distance = calcule_distance(p->x1, p->y1, p->x2, p->y2 + 1);
    }
    if (calcule_distance(p->x1, p->y1, p->x2 - 1, p->y2 + 1) < p->distance) {
        point = 4;
        p->distance = calcule_distance(p->x1, p->y1, p->x2 - 1, p->y2 + 1);
    }
    return find_direction_2(p, point);
}

int recalibration(int point, int orientation)
{
    if (point == 0)
        return 0;
    if (orientation == NORTH)
        return (point - 2 + 8) % 8;
    if (orientation == EAST)
        return point;
    if (orientation == SOUTH)
        return (point + 2) % 8;
    if (orientation == WEST)
        return (point + 4) % 8;
    return 0;
}

int calcule_shortened
(
    player_t *player, players_list_t *tmp, int direction, server_t *server
)
{
    if (tmp->value->x - player->x > server->map.width / 2 ||
        tmp->value->x - player->x < -(server->map.width / 2)) {
        direction = (direction + 4) % 8;
        if (direction == 0)
            direction = 8;
    }
    if (tmp->value->y - player->y > server->map.height / 2 ||
        tmp->value->y - player->y < -(server->map.height / 2)) {
        direction = (direction + 4) % 8;
        if (direction == 0)
            direction = 8;
    }
    return direction;
}

static void send_brodcast_graphic(server_t *server, player_t *player,
    const char *text)
{
    if (server->graphical_socket == -1)
        return;
    dprintf(server->graphical_socket, "pbc #%d %s\n", player->id, text);
}

static void set_position(position_t *pos, player_t *p, players_list_t *tmp)
{
    pos->x1 = p->x;
    pos->y1 = p->y;
    pos->x2 = tmp->value->x;
    pos->y2 = tmp->value->y;
}

static int do_continue(players_list_t *tmp, player_t *p,
    server_t *server, const char *text)
{
    if (tmp->value->team_id == DEFAULT_TEAM_ID) {
        return 1;
    }
    if (tmp->value->id == p->id) {
        send_brodcast_graphic(server, p, text);
        return 1;
    }
    return 0;
}

void broadcast_text(player_t *p, server_t *server, const char *text)
{
    players_list_t *tmp = server->players;
    position_t pos;
    int direction = 0;

    while (tmp != NULL) {
        if (do_continue(tmp, p, server, text)) {
            tmp = tmp->next;
            continue;
        }
        set_position(&pos, p, tmp);
        direction = find_direction(&pos);
        direction = recalibration(direction, tmp->value->direction);
        direction = calcule_shortened(p, tmp, direction, server);
        dprintf(tmp->value->socket, "message %d, %s\n", direction, text);
        tmp = tmp->next;
    }
    dprintf(p->socket, "ok\n");
    p->cooldown = 7;
}
