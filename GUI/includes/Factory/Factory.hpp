/*
** EPITECH PROJECT, 2024
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** Parser.hpp
*/

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include "../GameData/GameData.hpp"


enum class GraphicsCommand {
    MSZ,  // Taille de la carte -
    BCT,  // Contenu d'une tuile -
    TNA,  // Noms des équipes -
    PNW,  // Connexion d'un nouveau joueur -
    PPO,  // Position d'un joueur -
    PLV,  // Niveau d'un joueur -
    PIN,  // Inventaire d'un joueur -
    PDR,  // ressources dropping important
    PGT,  // Collecte de ressources important
    PDI,  // Death of a player important
    PIC,  // Début d'une incantation - important
    PIE,  // Fin d'une incantation - important -

    SGT, // time unit request bof
    PEX,  // Expulsion d'un joueur - osef
    SEG,  // End of game pas encore implementé
    SST, //time unit modification

};

// Enum pour les types de ressources
enum class ResourceType {
    FOOD,
    LINEMATE,
    DERAMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME
};


class CommandFactory {
public:
    static void executeCommand(GraphicsCommand cmd, const std::vector<std::string>& params, GameData& gameData) {
        switch (cmd) {
            case GraphicsCommand::PDI:
                gameData.DeathPlayer(params);
                break;
            case GraphicsCommand::MSZ:
                gameData.setMapSize(params);
                break;
            case GraphicsCommand::BCT:
                gameData.updateTile(params);
                break;
            case GraphicsCommand::PNW:
                gameData.addPlayer(params);
                break;
            case GraphicsCommand::TNA:
                gameData.setTeamName(params);
                break;
            case GraphicsCommand::PPO:
                gameData.UpdatePlayerPos(params);
                break;
            case GraphicsCommand::PLV:
                gameData.GetPlyerLvl(params);
                break;
            case GraphicsCommand::PIN:
                gameData.InventoryStatus(params);
                break;
            case GraphicsCommand::PEX:
                gameData.ExpulsePly(params);
                break;
            case GraphicsCommand::PIC:
                gameData.startIncantation(params);
                break;
            case GraphicsCommand::PIE:
                gameData.EndIncantation(params);
                break;
            case GraphicsCommand::PDR:
                gameData.DropRessource(params);
                break;
            case GraphicsCommand::PGT:
                gameData.CollecteRessource(params);
                break;
            case GraphicsCommand::SEG:
                gameData.EndGame(params);
                break;
            case GraphicsCommand::SGT:
                gameData.timeUnitReq(params);
                break;
            case GraphicsCommand::SST:
                gameData.timeUnitMod(params);
                break;
            default:
                std::cerr << "Unknown command" << std::endl;
                break;
        }
    }
private:

};
