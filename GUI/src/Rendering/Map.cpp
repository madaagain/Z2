/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** GUI.hpp
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include "APlayer.hpp"
#include "ATiles.hpp"
#include "Mouse.hpp"
#include "Window.hpp"

ATiles::ATiles(): _mapX(0), _mapY(0), _nbPlayers(0), _food(0), _linemate(0), _deraumere(0), _sibur(0), _mendiane(0), _phiras(0), _thystame(0)
{
}

ATiles::~ATiles()
{
}

void ATiles::drawGrid(sf::RenderWindow &window, int numRows, int numCols, int squareSize, MouseClick& mouse)
{
    completeFullBackground(window, squareSize);
    for (int i = -1; i <= numRows; ++i) {
        for (int j = -1; j <= numCols; ++j) {
            background_map(i , j, window, numRows, numCols, squareSize, mouse);
        }
    }
}

std::string ATiles::getTileInfo(int row, int col)
{
    (void)row;
    (void)col;
    return "\nFood: " +
        std::to_string(_food) + "\n" + "Linemate: " + std::to_string(_linemate) + "\n" +
        "Deraumere: " + std::to_string(_deraumere) + "\n" + "Sibur: " + std::to_string(_sibur) + "\n" +
        "Mendiane: " + std::to_string(_mendiane) + "\n" + "Phiras: " + std::to_string(_phiras) + "\n" +
        "Thystame: " + std::to_string(_thystame) + "\n";
}

void ATiles::completeFullBackground(sf::RenderWindow &window, int squareSize)
{
    (void)squareSize;
    if(!texture.loadFromFile("GUI/dataset/assets/map/ocean.png")) {
        std::cerr << "Error loading texture" << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setPosition(0, 0);
    sprite.setScale(2, 2);
    for (int i = 0; i < windowHeight; i += 50) {
        for (int j = 0; j < windowWidth; j += 50) {
            sprite.setPosition(j, i);
            window.draw(sprite);
        }
    }
    window.draw(sprite);
}

void ATiles::background_map(int i, int j, sf::RenderWindow &window, int numRows, int numCols, int squareSize, MouseClick& mouse)
{
    std::string key;

    if (i == -1) {
        if (j == -1) key = "-1,-1";
        else if (j == numCols) key = "-1,numCols";
        else key = "-1,mid";
    } else if (j == -1) {
        if (i == numRows) key = "numRows,-1";
        else key = "mid,-1";
    } else if (i == numRows) {
        if (j == numCols) key = "numRows,numCols";
        else key = "numRows,mid";
    } else if (j == numCols) {
        key = "mid,numCols";
    } else if (i == 0) {
        if (j == 0) key = "0,0";
        else if (j == numCols - 1) key = "0,numCols-1";
        else key = "0,mid";
    } else if (j == 0) {
        if (i == numRows - 1) key = "numRows-1,0";
        else key = "mid,0";
    } else if (i == numRows - 1) {
        if (j == numCols - 1) key = "numRows-1,numCols-1";
        else key = "numRows-1,mid";
    } else if (j == numCols - 1) {
        key = "mid,numCols-1";
    } else {
        key = "default";
    }

    texture.loadFromFile(textureMap[key]);
    textureSize = texture.getSize();
    sprite.setTexture(texture);
    sprite.setPosition(j * squareSize + mouse.yOffset, i * squareSize + mouse.xOffset);
    sprite.setScale(1.8, 1.8);
    window.draw(sprite);
}

void ATiles::createPlayer(int id,int y, int x, int orientation, int lvl, std::string teamName, std::shared_ptr<Window> window)
{
    if (window == nullptr) {
        sf::RenderWindow* win = nullptr;
        _Players.emplace_back(std::make_shared<Player>(id,y,x,orientation,lvl,teamName, win));
    } else {
        _Players.emplace_back(std::make_shared<Player>(id,y,x,orientation,lvl,teamName, &window->window_));
    }
}

void ATiles::setfood(int nbr)
{
    _food = nbr;
};

void ATiles::setNbSupPlayers()
{
    _nbPlayers++;
};

void ATiles::setNbMinusPlayers()
{
    _nbPlayers--;
};

void  ATiles::setLinemate(int nbr)
{
    _linemate = nbr;
};

void  ATiles::setDeraumere(int nbr)
{
    _deraumere = nbr;
};

void  ATiles::setsibur(int nbr)
{
    _sibur = nbr;
};

void  ATiles::setmendiane(int nbr)
{
    _mendiane = nbr;
};

void  ATiles::setphiras(int nbr)
{
    _phiras = nbr;
};

void  ATiles::setthystame(int nbr)
{
    _thystame = nbr;
};

int ATiles::getfood()
{
    return _food;
};

int ATiles::getLinemate()
{
    return _linemate;
};

int ATiles::getDeraumere()
{
    return _deraumere;
};

int ATiles::getSibur()
{
    return _sibur;
};

int ATiles::getMendiane()
{
    return _mendiane;
};

int ATiles::getPhiras()
{
    return _phiras;
};

int ATiles::getThystame()
{
    return _thystame;
};

void ATiles::addRessourcesTiles(int nbr)
{
    if (nbr == 0)
        _food++;
    if (nbr == 1)
        _linemate++;
    if (nbr == 2)
        _deraumere++;
    if (nbr == 3)
        _sibur++;
    if (nbr == 4)
        _mendiane++;
    if (nbr == 0)
        _phiras++;
    if (nbr == 0)
        _thystame++;
}

void ATiles::dropRessourcesTiles(int nbr)
{
    if (nbr == 0)
        _food--;
    if (nbr == 1)
        _linemate--;
    if (nbr == 2)
        _deraumere--;
    if (nbr == 3)
        _sibur--;
    if (nbr == 4)
        _mendiane--;
    if (nbr == 0)
        _phiras--;
    if (nbr == 0)
        _thystame--;
}
