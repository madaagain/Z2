/*
** EPITECH PROJECT, 2024
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description: Chaque classe a une responsabilité claire et unique,
** ce qui aide à suivre les principes de conception SOLID,
** en particulier le principe de responsabilité unique.
** GUI.hpp
*/

#ifndef GAMEDATA_H_
    #define GAMEDATA_H_

class GameData;
class Window;
class ATiles;

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "IEntity.hpp"
#include "ATiles.hpp"

#define UNUSED(x) (void)(x)

class GameData {
public:
    GameData(){};
    ~GameData(){};
    std::vector<int> getTextureForPlayer(const std::vector<std::string>& params);
    void setMapSize(const std::vector<std::string>& params);
    void updateTile(const std::vector<std::string>& params);
    void addPlayer(const std::vector<std::string>& params);
    void setTeamName(const std::vector<std::string>& params);
    void UpdatePlayerPos(const std::vector<std::string>& params);
    void GetPlyerLvl(const std::vector<std::string>& params);
    void InventoryStatus(const std::vector<std::string>& params);
    void ExpulsePly(const std::vector<std::string>& params);
    void startIncantation(const std::vector<std::string>& params);
    void EndIncantation(const std::vector<std::string>& params);
    void DropRessource(const std::vector<std::string>& params);
    void CollecteRessource(const std::vector<std::string>& params);
    void DeathPlayer(const std::vector<std::string>& params);
    void EndGame(const std::vector<std::string>& params);
    void timeUnitReq(const std::vector<std::string>& params);
    void timeUnitMod(const std::vector<std::string>& params);
    int getMapWidth() const; /** @brief Getters pour la map en X */
    int getMapHeight() const; /** @brief Getters pour la map en Y */
    void initialise_map(const std::vector<std::string>& params);
    std::vector<std::vector<std::unique_ptr<ATiles>>> tiles_map;
    int mapWidth, mapHeight = -1;
    bool EndOfGame = false;
    std::shared_ptr<Window> window = nullptr;
private:
    int playerNumbers;
    int timeUnit;
};

#endif /* GAMEDATA.HPP */
