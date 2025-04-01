/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** parse_arg.cpp
*/

#include "ParseArg.hpp"

void ParseArg::handle_port(std::string arg, Client &client)
{
    if (stoi(arg))
        client.setPort(stoi(arg));
    else
        throw Lib::Critical("Port must be a number");
    return;
}

void ParseArg::handle_machine(std::string arg, Client &client)
{
    client.setIp(arg);
    return;
}

int ParseArg::parse(int ac, char **av, Client &client)
{
    std::string arg1;
    int check = 0;

    if (ac != 2 && ac != 5)
        throw Lib::Critical("Wrong argument");
    arg1 = av[1];
    if (arg1 == "--help") {
        dashHelp();
        return BREAK;
    }
    for (int i = 0; av[i] != nullptr; i++) {
        arg1 = av[i];
        if (arg1 == "-p" && av[i + 1] != nullptr) {
            handle_port(av[i + 1], client);
            check = check + 1;
        }
        if (arg1 == "-h" && av[i + 1] != nullptr) {
            handle_machine(av[i + 1], client);
            check = check + 1;
        }
    }
    if (check != 2) {
        throw Lib::Critical("Wrong number of arguments");
    }
    return 0;
}

void ParseArg::dashHelp()
{
    std::cout << "USAGE: ./zappy_gui -p port -h machine" << std::endl;
    return;
}
