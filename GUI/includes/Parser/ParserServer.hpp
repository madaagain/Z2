/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ParserServer
*/

#ifndef PARSERSERVER_HPP_
#define PARSERSERVER_HPP_
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "GameData.hpp"

class ParserServer {
    public:
        ParserServer();
        ~ParserServer();
        static void parse(const std::string& command, GameData& gameData);

    protected:
    private:
};

#endif /* !PARSERSERVER_HPP_ */
