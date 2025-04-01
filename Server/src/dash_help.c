/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** dash_help.c
*/

#include "server.h"
#include <stdio.h>
#include <string.h>

void print_help(void)
{
    printf("USAGE: ./zappy_server -p port -x width -y height -n");
    printf("name1 name2 ... -c clientsNb -f freq\n");
    printf("\tport\t\tis the port number\n");
    printf("\twidth\t\tis the width of the world\n");
    printf("\theight\t\tis the height of the world\n");
    printf("\tnameX\t\tis the name of the team X\n");
    printf("\tclientsNb\t\tis the number of authorized clients per team\n");
    printf("\tfreq\t\tis the reciprocal of ");
    printf("time unit for execution of actions\n");
}

void dash_help(int ac, char **av)
{
    if (ac < 2)
        return;
    if (!strcmp(av[1], "-help")) {
        print_help();
        return;
    }
}
// printf("\n\tauto-start\tdoes the greeting is send automaticly\n");
// printf("\tdisplay-eggs\teggs are visible and destructible\n");
