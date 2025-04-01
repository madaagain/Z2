

#include "server.h"
#include "args.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void init_struct(arguments_t *arg)
{
    arg->clientsNb = DEFAULT_CLIENTNB;
    arg->freq = DEFAULT_FREQUENCE;
    arg->height = DEFAULT_HEIGHT;
    arg->width = DEFAULT_WIDHT;
    arg->name = malloc(5 * sizeof(char *));
    if (arg->name == NULL) {
        perror("malloc default name fail");
        return;
    }
    arg->name[0] = strdup(DEFAULT_TEAM_NAME[0]);
    arg->name[1] = strdup(DEFAULT_TEAM_NAME[1]);
    arg->name[2] = strdup(DEFAULT_TEAM_NAME[2]);
    arg->name[3] = strdup(DEFAULT_TEAM_NAME[3]);
    arg->name[4] = NULL;
    arg->port = DEFAULT_PORT;
}

int count(int i, char **av, int ac, int name_count)
{
    if (!strcmp(av[i], "-n")) {
        while (i + 1 < ac && av[i + 1][0] != '-') {
            name_count++;
            i++;
        }
        return name_count;
    }
    return 0;
}

static int malloc_name(int ac, char **av, arguments_t *arg)
{
    int name_count = 0;

    for (int i = 1; i < ac; ++i) {
        name_count = count(i, av, ac, name_count);
        if (name_count != 0)
            break;
    }
    if (name_count > 0) {
        for (int i = 0; i < 4; i++)
            free(arg->name[i]);
        free(arg->name);
        arg->name = malloc((name_count + 1) * sizeof(char *));
        if (!arg->name) {
            perror("error malloc : ");
            return ERROR;
        }
        arg->name[name_count] = NULL;
    }
    return 0;
}

static int handle_freq(
    arguments_t *arg, char *optarg, char **av, int ac
)
{
    UNUSED(av);
    UNUSED(ac);
    arg->freq = atoi(optarg);
    if (arg->freq <= 0)
        return ERROR;
    return 0;
}

static int call_pointer_to_stock_arg(
    args_t main_arg, arguments_t *arg, option_handler_t array[]
)
{
    int i = main_arg.i;

    if (optarg == NULL)
        return 0;
    if (main_arg.opt == array[i].opt) {
        if (array[i].handler(arg, optarg, main_arg.av, main_arg.ac) == ERROR)
            return 0;
        return 1;
    }
    return 2;
}

int loop_to_pars_arg(
    args_t main_arg, arguments_t *arg, option_handler_t array[]
)
{
    int handled = 0;

    for (int i = 0; array[i].opt != 0; i++) {
        main_arg.i = i;
        handled = call_pointer_to_stock_arg(main_arg, arg, array);
        if (handled != 2)
            break;
    }
    if (!handled) {
        print_help();
        return ERROR;
    }
    return 0;
}

static int call_loop_pointer(
    args_t main_arg, option_handler_t array[], arguments_t *arg
)
{
    int check = 0;

    main_arg.opt = getopt(main_arg.ac, main_arg.av, "p:x:y:n:c:f:");
    if (main_arg.opt == -1)
        return BREAK;
    check = loop_to_pars_arg(main_arg, arg, array);
    if (check == BREAK)
        return BREAK;
    if (check == ERROR)
        return ERROR;
    return 0;
}

int loop_pointer_to_function(
    int ac, char **av, arguments_t *arg, option_handler_t array[]
)
{
    int check = 0;
    args_t main_arg;

    main_arg.ac = ac;
    main_arg.av = av;
    opterr = 0;
    init_struct(arg);
    if (malloc_name(ac, av, arg) == ERROR)
        return ERROR;
    while (1) {
        check = call_loop_pointer(main_arg, array, arg);
        if (check == BREAK)
            break;
        if (check == ERROR)
            return ERROR;
    }
    return 0;
}

int parse_arg(int ac, char **av, arguments_t *arg)
{
    option_handler_t array[] = {
        {'p', handle_port},
        {'x', handle_width},
        {'y', handle_height},
        {'n', handle_name},
        {'c', handle_clients_nb},
        {'f', handle_freq},
        {0, NULL}
    };

    if (ac == 2 && (!strcmp(av[1], "--help") || !strcmp(av[1], "-h"))) {
        print_help();
        return HELP;
    }
    if (loop_pointer_to_function(ac, av, arg, array) == ERROR)
        return ERROR;
    return 0;
}
