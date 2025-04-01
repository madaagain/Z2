
#include "../../includes/Parser/ParserServer.hpp"
#include "../../includes/Factory/Factory.hpp"

ParserServer::ParserServer()
{
    ;
}

ParserServer::~ParserServer()
{
    ;
}

void ParserServer::parse(const std::string& command, GameData& gameData)
{
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "msz") {

        std::vector<std::string> params;
        std::string posX;
        std::string posY;

        iss >> posX >> posY;

        /** @brief: Ici j'ajoute la position X, Y aux vecteur
         * @Mikael et @Pablo */
        params.push_back(posX);
        params.push_back(posY);

        /** @brief J'envoie la commande dans la factory */
        CommandFactory::executeCommand(GraphicsCommand::MSZ, params, gameData);

    } else if (cmd == "bct") {

        std::vector<std::string> params;
        std::string posX;
        std::string posY;
        std::string ressources;

        iss >> posX >> posY;

        params.push_back(posX);
        params.push_back(posY);

        /** @brief Je paracours le stream de la string pour voir le nombres d'arguments de ressources
         * @mikael et @pablo
         * @attention Important line of code
        */

        while (iss >> ressources) {
            params.push_back(ressources);
        }
        CommandFactory::executeCommand(GraphicsCommand::BCT, params, gameData);
    } else if (cmd == "tna") {

        std::vector<std::string> params;
        std::string teamName;

        while(iss >> teamName) {
            params.push_back(teamName);
        }
        CommandFactory::executeCommand(GraphicsCommand::TNA, params, gameData);
    } else if (cmd == "pnw") {

        std::vector<std::string> params;
        std::string playerNum;
        std::string posX;
        std::string posY;
        std::string orient;
        std::string level;
        std::string teamName;

        iss >> playerNum >> posX >> posY >> orient >> level >> teamName;
        params.push_back(playerNum);
        params.push_back(posX);
        params.push_back(posY);
        params.push_back(orient);
        params.push_back(level);
        params.push_back(teamName);


        CommandFactory::executeCommand(GraphicsCommand::PNW, params, gameData);

    } else if (cmd == "ppo") {

        std::vector<std::string> params;
        std::string playerNum;
        std::string posX;
        std::string posY;
        std::string orient;

        iss >> playerNum >> posX >> posY >> orient;

        params.push_back(playerNum);
        params.push_back(posX);
        params.push_back(posY);
        params.push_back(orient);

        CommandFactory::executeCommand(GraphicsCommand::PPO, params, gameData);

    } else if (cmd == "pin") {

        std::vector<std::string> params;
        std::string playerNum;
        std::string posX;
        std::string posY;
        std::string ressources;

        iss >> playerNum >> posX >> posY;

        params.push_back(playerNum);
        params.push_back(posX);
        params.push_back(posY);

        while(iss >> ressources) {
            params.push_back(ressources);
        }
        CommandFactory::executeCommand(GraphicsCommand::PIN, params, gameData);

    } else if (cmd == "plv") {

        std::vector<std::string> params;
        std::string playerNum;
        std::string level;

        iss >> playerNum >> level;

        params.push_back(playerNum);
        params.push_back(level);

        CommandFactory::executeCommand(GraphicsCommand::PLV, params, gameData);
    } else if (cmd == "pex") {

        std::vector<std::string> params;
        std::string playerNum;

        iss >> playerNum;
        params.push_back(playerNum);
        CommandFactory::executeCommand(GraphicsCommand::PEX, params, gameData);

    } else if (cmd == "pic") {

        std::vector<std::string> params;
        std::string playerNum;
        std::string posX;
        std::string posY;
        std::string level;

        iss >> posX >> posY >> level;

        params.push_back(posX);
        params.push_back(posY);
        params.push_back(level);

        while (iss >> playerNum) {
            params.push_back(playerNum);
        }

        CommandFactory::executeCommand(GraphicsCommand::PIC, params, gameData);
    } else if (cmd == "seg") {
        gameData.EndOfGame = true;
    } else if (cmd == "pdi") {
            std::vector<std::string> params;
            std::string playerNum;

            iss >> playerNum;
            params.push_back(playerNum);

            CommandFactory::executeCommand(GraphicsCommand::PDI, params, gameData);
    }
}
