/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** handle_struct_arg.c
*/

#include "server.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

int handle_port(
    arguments_t *arg, char *optarg, char **av, int ac
)
{
    UNUSED(ac);
    UNUSED(av);
    arg->port = atoi(optarg);
    if (arg->port == 0)
        return ERROR;
    return 0;
}

int handle_width(
    arguments_t *arg, char *optarg, char **av, int ac
)
{
    UNUSED(ac);
    UNUSED(av);
    arg->width = atoi(optarg);
    if (arg->width <= 0)
        return ERROR;
    return 0;
}

int handle_height(
    arguments_t *arg, char *optarg, char **av, int ac
)
{
    UNUSED(ac);
    UNUSED(av);
    arg->height = atoi(optarg);
    if (arg->height <= 0)
        return ERROR;
    return 0;
}

static int check_team_name(arguments_t *arg, char *name)
{
    for (int i = 0; i < arg->name_count; i++) {
        if (strcmp(arg->name[i], name) == 0) {
            fprintf(stderr, "Error: Duplicate team name '%s'\n", name);
            return ERROR;
        }
        if (strcmp(arg->name[i], "GRAPHIC") == 0) {
            fprintf(stderr, "Error: Team name 'GRAPHIC' is reserved\n");
            return ERROR;
        }
    }
    return 0;
}

int handle_name(arguments_t *arg, char *optarg, char **av, int ac)
{
    int i = 0;

    UNUSED(optarg);
    arg->name_count = 0;
    if (!arg->name)
        return ERROR;
    optind = optind - 1;
    while (optind < ac && av[optind][0] != '-') {
        if (check_team_name(arg, av[optind]) == ERROR)
            return ERROR;
        arg->name[i] = strdup(av[optind]);
        i++;
        arg->name_count++;
        optind++;
    }
    arg->name[i] = NULL;
    return 0;
}

int handle_clients_nb(
    arguments_t *arg, char *optarg, char **av, int ac
)
{
    UNUSED(ac);
    UNUSED(av);
    arg->clientsNb = atoi(optarg);
    if (arg->clientsNb == 0)
        return ERROR;
    return 0;
}
