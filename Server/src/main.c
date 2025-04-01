/*
** EPITECH PROJECT, 2024
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** main
*/

#include "server.h"
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int global_catch(int _catch)
{
    static int catch;

    if (catch == 1)
        return catch;
    if (_catch != 0) {
        catch = _catch;
        return catch;
    }
    if (_catch == 0) {
        catch = _catch;
        return catch;
    }
    return catch;
}

void handle_sigint(int)
{
    fprintf(stderr, "Server shutdown\n");
    global_catch(1);
    return;
}

int main(int ac, char **av)
{
    arguments_t arg;
    int return_value = 0;
    int check = 0;

    dash_help(ac, av);
    check = parse_arg(ac, av, &arg);
    if (check == ERROR)
        return ERROR;
    if (check == HELP)
        return 0;
    if (signal(SIGINT, handle_sigint) == SIG_ERR)
        return ERROR;
    print_info(arg);
    return_value = created_socket(&arg);
    for (int i = 0; arg.name[i] != NULL; i++)
        free(arg.name[i]);
    free(arg.name);
    return return_value;
}
