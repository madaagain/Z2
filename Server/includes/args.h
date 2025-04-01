/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** args.h
*/

#ifndef ARGS_H_
    #define ARGS_H_

    #define DEFAULT_CLIENTNB (5)
    #define DEFAULT_FREQUENCE (100)
    #define DEFAULT_HEIGHT (10)
    #define DEFAULT_WIDHT (10)
    #define DEFAULT_PORT (4242)
    #define DEFAULT_TEAM_NAME ((char *[]){"Team1", "Team2", "Team3", "Team4"})

typedef struct args_s {
    int ac;
    char **av;
    int opt;
    int i;
} args_t;

typedef struct position_s {
    int x1;
    int y1;
    int x2;
    int y2;
    int point;
    double distance;
} position_t;

#endif /* !ARGS_H_ */
