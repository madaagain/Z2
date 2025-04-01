

#include "game_info.h"
#include "server.h"

void add_command_to_queue(player_t *player, char *command)
{
    queue_t *new = NULL;
    queue_t *current = player->command_queue;

    if (player->queue_size >= 10)
        return;
    new = malloc(sizeof(queue_t));
    if (new == NULL)
        return;
    new->command = command;
    new->next = NULL;
    if (current == NULL) {
        player->command_queue = new;
    } else {
        while (current->next != NULL)
            current = current->next;
        current->next = new;
    }
    player->queue_size++;
}

char *pop_command_from_queue(player_t *player)
{
    queue_t *current = player->command_queue;
    char *command = NULL;

    if (current == NULL)
        return NULL;
    command = strdup(current->command);
    player->command_queue = current->next;
    free(current->command);
    free(current);
    player->queue_size--;
    return command;
}
