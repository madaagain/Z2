/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** Teams.hpp
*/

#ifndef TEAMS_H_
    #define TEAMS_H_

#include <string>

class Teams
{
    public:
        Teams();
        ~Teams();
    private:
        std::string teamsName;
        int TeamsId;
        int nbTeamsPlayers; // nombre de joueur dans cette équipe. Mettre un Player[] à la place ?
        //Player player[];
};

#endif /* Teams.HPP */
