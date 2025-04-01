/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** GUI.hpp
*/

#ifndef TILES_H_
    #define TILES_H_

class ATiles;
class MouseClick;
class GameData;
class Window;

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <string>
#include "AStone.hpp"
#include "AItems.hpp"
#include "APlayer.hpp"

class ATiles : public AItems
{
    public:
        ATiles();
        ~ATiles();
        void drawGrid(sf::RenderWindow &window, int numRows, int numCols, int squareSize, MouseClick& mouse);
        std::string getTileInfo(int row, int col);
        int windowWidth;
        int windowHeight;
        void setNbMinusPlayers();
        void setfood(int nbr);
        void setLinemate(int nbr);
        void setDeraumere(int nbr);
        void setsibur(int nbr);
        void setmendiane(int nbr);
        void setphiras(int nbr);
        void setthystame(int nbr);
        void addRessourcesTiles(int nbr);
        void dropRessourcesTiles(int nbr);
        int getfood();
        int getLinemate();
        int getDeraumere();
        int getSibur();
        int getMendiane();
        int getPhiras();
        int getThystame();
        int getNbPlayer(){
            return _nbPlayers;
        }
        void setNbSupPlayers();
        void createPlayer(int id,int y, int x, int orientation, int lvl, std::string teamName, std::shared_ptr<Window> window);
        std::vector<std::shared_ptr<Player>> _Players;
        void completeFullBackground(sf::RenderWindow &window, int squareSize);
        void background_map(int i, int j, sf::RenderWindow &window, int numRows, int numCols, int squareSize, MouseClick& mouse);
        sf::Sprite sprite;
        sf::Texture texture;
        int _mapX;
        int _mapY;
        int _nbPlayers;

    private:
        int _food;
        int _linemate;
        int _deraumere;
        int _sibur;
        int _mendiane;
        int _phiras;
        int _thystame;
        std::map<std::string, std::string> textureMap = {
            {"-1,-1", "GUI/dataset/assets/map/ocean_terrain_top_left.png"},
            {"-1,mid", "GUI/dataset/assets/map/ocean_terrain_top_middle.png"},
            {"-1,numCols", "GUI/dataset/assets/map/ocean_terrain_top_right.png"},
            {"mid,-1", "GUI/dataset/assets/map/ocean_terrain_left_middle.png"},
            {"numRows,-1", "GUI/dataset/assets/map/ocean_terrain_bottom_left.png"},
            {"0,0", "GUI/dataset/assets/map/grass_left_top.png"},
            {"0,numCols-1", "GUI/dataset/assets/map/grass_right_top.png"},
            {"numRows-1,0", "GUI/dataset/assets/map/grass_left_down.png"},
            {"numRows-1,numCols-1", "GUI/dataset/assets/map/grass_right_down.png"},
            {"numRows-1,mid", "GUI/dataset/assets/map/grass_middle_down.png"},
            {"mid,0", "GUI/dataset/assets/map/grass_middle_left.png"},
            {"mid,numCols-1", "GUI/dataset/assets/map/grass_middle_right.png"},
            {"numRows,numCols", "GUI/dataset/assets/map/ocean_terrain_bottom_right.png"},
            {"mid,numCols", "GUI/dataset/assets/map/ocean_terrain_right_middle.png"},
            {"numRows,mid", "GUI/dataset/assets/map/ocean_terrain_bottom_middle.png"},
            {"0,mid", "GUI/dataset/assets/map/grass_middle_top.png"},
            {"default", "GUI/dataset/assets/map/grass.png"}
        };
        sf::Vector2u textureSize;
        sf::Vector2f initialViewOffset;
};

#endif /* GUI.HPP */
