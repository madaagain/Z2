
#include "server.h"
#include <stdio.h>

int count_elements(char **array)
{
    int i = 0;

    while (array[i] != NULL)
        i++;
    return i;
}

void print_team(arguments_t arg)
{
    int nb_names = count_elements(arg.name);

    fprintf(stderr, "Teams [%d] :\n", nb_names);
    for (int i = 0; i < nb_names; i++) {
        fprintf(stderr, "Team number %d : [%s]\n", i + 1, arg.name[i]);
    }
    return;
}

void print_info(arguments_t arg)
{
    fprintf(stderr, "===============Zappy Server================\n");
    fprintf(stderr, "freq = %d\n", arg.freq);
    fprintf(stderr, "width = %d\n", arg.width);
    fprintf(stderr, "height = %d\n", arg.height);
    fprintf(stderr, "port = %d\n", arg.port);
    fprintf(stderr, "clients per team = %d\n", arg.clientsNb);
    print_team(arg);
    return;
}
