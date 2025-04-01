/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** GUI.hpp
*/

#ifndef MOUSE_H_
    #define MOUSE_H_

    class MouseClick;

    #include <iostream>
    #include <string>
    #include <SFML/Graphics.hpp>
    #include "GameData.hpp"

class MouseClick {
    public:
        MouseClick();
        ~MouseClick();
        void handleMouseClick(int x, int y, int numRows, int numCols, int squareSize, sf::RenderWindow &window_, GameData& gameData_);
        bool dragging;
        int clickedRow_;
        int clickedCol_;
        sf::Vector2i initialMousePos;
        sf::Vector2i initialWindowPos;
        sf::Vector2i currentMousePos;
        sf::Vector2f deltaMousePos;
        std::string cellInfo_;
        int xOffset;
        int yOffset;
    private:
        sf::Vector2f worldPos;
        sf::Font font;
        sf::View view;
        sf::Vector2f initialViewOffset;
};

#endif
