/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameData
*/

#include "GameData.hpp"
#include "APlayer.hpp"
#include "iostream"

std::vector<int> GameData::getTextureForPlayer(const std::vector<std::string>& params)
{
    UNUSED(params);
    return std::vector<int>();
}

int GameData::getMapWidth() const
{
    return mapWidth;
}

int GameData::getMapHeight() const
{
    return mapHeight;
}

void GameData::setMapSize(const std::vector<std::string>& params)
{
    mapWidth = std::stoi(params[0]);
    mapHeight = std::stoi(params[1]);
    initialise_map(params);
}

void GameData::initialise_map(const std::vector<std::string>& params)
{
    int numRows = std::stoi(params[0]);
    int numCols = std::stoi(params[1]);
    tiles_map.resize(numRows);
    for (int i = 0; i < numRows; ++i) {
        tiles_map[i].resize(numCols);
    }

    for (int y = 0; y < numRows; ++y) {
        for (int x = 0; x < numCols; ++x) {
            if (tiles_map[y][x] == nullptr)
                tiles_map[y][x] = std::make_unique<ATiles>();
        }
    }
}

void GameData::updateTile(const std::vector<std::string>& params)
{
    /** @brief Mettre à jour les ressources ou l'état de la tuile à ces coordonnées
     * Cela pourrait signifie la MAJ des entités graphiques ou logiques sur cette tuile
     * */
    int y = std::stoi(params[0]);
    int x = std::stoi(params[1]);
    if (mapWidth < y || mapHeight < x)
        std::cerr << "not allocate space" << std::endl;
    tiles_map[y][x]->setfood(std::stoi(params[2]));
    tiles_map[y][x]->setLinemate(std::stoi(params[3]));
    tiles_map[y][x]->setDeraumere(std::stoi(params[4]));
    tiles_map[y][x]->setsibur(std::stoi(params[5]));
    tiles_map[y][x]->setmendiane(std::stoi(params[6]));
    tiles_map[y][x]->setphiras(std::stoi(params[7]));
    tiles_map[y][x]->setthystame(std::stoi(params[8]));
}

void GameData::addPlayer(const std::vector<std::string>& params)
{
    int y = std::stoi(params[1]);
    int x = std::stoi(params[2]);

    std::string numberStr = params[0].substr(1);
    int playerID = std::stoi(numberStr);
    tiles_map[y][x]->createPlayer(playerID, y, x, std::stoi(params[3]), std::stoi(params[4]), params[5], window);
    tiles_map[y][x]->setNbSupPlayers();
}


void GameData::setTeamName(const std::vector<std::string>& params)
{
    UNUSED(params);
    return ;
}

void GameData::UpdatePlayerPos(const std::vector<std::string>& params)
{
    std::string numberStr = params[0].substr(1);
    int player_nbr = std::stoi(numberStr);
    int new_pos_y = std::stoi(params[1]);
    int new_pos_x = std::stoi(params[2]);
    int new_orien = std::stoi(params[3]);

    std::shared_ptr<Player> player = nullptr;
    bool playerFound = false;

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            auto& players = tiles_map[y][x]->_Players;
            if (players.empty())
                continue;
            for (auto it = players.begin(); it != players.end(); ++it) {
                if (it->get() == nullptr)
                    continue;
                if (it->get()->getId() == player_nbr) {
                    player = *it;
                    players.erase(it);
                    playerFound = true;
                    tiles_map[y][x]->setNbMinusPlayers();
                    break;
                }
            }
            if (playerFound)
                break;
        }
        if (playerFound)
            break;
    }

    if (playerFound) {
        player->setOrientation(new_orien);
        tiles_map[new_pos_y][new_pos_x]->setNbSupPlayers();
        tiles_map[new_pos_y][new_pos_x]->_Players.emplace_back(player);
        std::cout << "Player " << player_nbr << " moved to (" << new_pos_y << ", " << new_pos_x << ")" << std::endl;
    } else {
        std::cerr << "Player " << player_nbr << " not found on the map." << std::endl;
    }
}


void GameData::GetPlyerLvl(const std::vector<std::string>& params)
{
    /** @brief return the level of the player  */
    std::string numberStr = params[0].substr(1);
    int player_nbr = std::stoi(numberStr);
    int lvl = std::stoi(params[1]);

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            auto& players = tiles_map[y][x]->_Players;
            for (auto it = players.begin(); it != players.end(); it++) {
                if (it->get()->getId() == player_nbr) {
                    it->get()->set_level(lvl);
                    break;
                }
            }
        }
    }
    return ;
}

void GameData::InventoryStatus(const std::vector<std::string>& params)
{
    std::string numberStr = params[0].substr(1);
    int player_nbr = std::stoi(numberStr);
    int y = std::stoi(params[1]);
    int x = std::stoi(params[2]);
    if (mapWidth < y || mapHeight < x)
        std::cerr << "not allocate space" << std::endl;

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            auto& players = tiles_map[y][x]->_Players;
            for (auto it = players.begin(); it != players.end(); it++) {
                if (it->get()->getId() == player_nbr) {
                    it->get()->setfood(std::stoi(params[3]));
                    it->get()->setLinemate(std::stoi(params[4]));
                    it->get()->setDeraumere(std::stoi(params[5]));
                    it->get()->setsibur(std::stoi(params[6]));
                    it->get()->setmendiane(std::stoi(params[7]));
                    it->get()->setphiras(std::stoi(params[8]));
                    it->get()->setthystame(std::stoi(params[9]));
                    break;
                }
            }
        }
    }
    return ;
}

void GameData::ExpulsePly(const std::vector<std::string>& params)
{
    UNUSED(params);
    ;
}

void GameData::startIncantation(const std::vector<std::string>& params)
{
    int how_many = params.size();
    std::vector<int> nbr_of_player;

    if (how_many > 3) {
        for (int a = how_many - 3; a < how_many; ++a) {
            std::string numberStr = params[0];
            int player_nbr = std::stoi(numberStr);
            nbr_of_player.push_back(player_nbr);
        }
        nbr_of_player.push_back(-2);
    }

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            auto& players = tiles_map[y][x]->_Players;
            for (auto it = players.begin(); it != players.end(); ++it) {
                for (int i = 0; nbr_of_player[i] != -2; ++i) {
                    if ((*it)->getId() == nbr_of_player[i]) {
                        (*it)->setIncantation();
                        break;
                    }
                }
            }
        }
    }
}

void GameData::EndIncantation(const std::vector<std::string>& params)
{
    UNUSED(params);
    ;
}

void GameData::DeathPlayer(const std::vector<std::string>& params)
{
    std::string numberStr = params[0].substr(1);
    int player_nbr = std::stoi(numberStr);

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            auto& players = tiles_map[y][x]->_Players;
            for (auto it = players.begin(); it != players.end(); it++) {

                if (it->get()->getId() == player_nbr) {
                    players.erase(it);
                    tiles_map[y][x]->setNbMinusPlayers();
                    return; 
                }
            }
        }
    }
}


void GameData::EndGame(const std::vector<std::string>& params)
{
    UNUSED(params);
    this->EndOfGame = true;
}

void GameData::CollecteRessource(const std::vector<std::string>& params)
{
    std::string numberStr = params[0].substr(1);
    int player_nbr = std::stoi(numberStr);
    int ressources_nbr = std::stoi(params[1]);

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            auto& players = tiles_map[y][x]->_Players;
            for (auto it = players.begin(); it != players.end(); it++) {
                if (it->get()->getId() == player_nbr) {
                    it->get()->CollectRessources(ressources_nbr);
                    tiles_map[y][x]->dropRessourcesTiles(ressources_nbr);
                    break;
                }
            }
        }
    }
    return ;
}

void GameData::DropRessource(const std::vector<std::string>& params)
{
    std::string numberStr = params[0].substr(1);
    int player_nbr = std::stoi(numberStr);
    int ressources_nbr = std::stoi(params[1]);

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            auto& players = tiles_map[y][x]->_Players;
            for (auto it = players.begin(); it != players.end(); it++) {
                if (it->get()->getId() == player_nbr) {
                    it->get()->dropRessources(ressources_nbr);
                    tiles_map[y][x]->addRessourcesTiles(ressources_nbr);
                    break;
                }
            }
        }
    }
    return ;
}


void GameData::timeUnitReq(const std::vector<std::string> &params)
{
    UNUSED(params);
    ;
}

void GameData::timeUnitMod(const std::vector<std::string> &params)
{
    UNUSED(params);
    ;
}
