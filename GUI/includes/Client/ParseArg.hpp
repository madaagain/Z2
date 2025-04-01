/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** ParseArg.hpp
*/

#pragma once

    #define ERROR (84)
    #define BREAK (18)

#include "Client.hpp"
#include "error/Critical.hpp"
#include <iostream>

// -p port -h machine

class ParseArg {
    public:
        ParseArg() = default;
        ~ParseArg() = default;

        int parse(int ac, char **av, Client &client);

    private:
        void dashHelp();
        void handle_machine(std::string arg, Client &client);
        void handle_port(std::string arg, Client &client);
};
