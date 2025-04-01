/*
** EPITECH PROJECT, 2024
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** main
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Client.hpp"
#include "ParseArg.hpp"
#include "GameData.hpp"
#include "Window.hpp"
#include "ParserServer.hpp"

int main(int ac, char **av)
{
    GameData gameData;

    try {
        ParseArg parse;
        Client client;

        if (parse.parse(ac, av, client) == BREAK)
            return 0;
        client.connectToServer();
        if (client.infiniteLoop() == ERROR)
            return ERROR;
        return 0;
    } catch (const std::exception &exeption) {
        std::cerr << exeption.what() << std::endl;
        return ERROR;
    }
}
