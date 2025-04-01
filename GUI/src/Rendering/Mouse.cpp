

#include <iostream>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include "Mouse.hpp"
#include "ATiles.hpp"

MouseClick::MouseClick()
{
    clickedRow_ = -1;
    clickedCol_ = -1;
}

MouseClick::~MouseClick()
{
}

void MouseClick::handleMouseClick(int x, int y, int numRows, int numCols, int squareSize, sf::RenderWindow& window_, GameData& gameData_)
{
    worldPos = window_.mapPixelToCoords(sf::Vector2i(x, y));

    if (worldPos.x >= yOffset && worldPos.x < yOffset + numCols * squareSize &&
        worldPos.y >= xOffset && worldPos.y < xOffset + numRows * squareSize) {
        clickedRow_ = static_cast<int>((worldPos.y - xOffset) / squareSize);
        clickedCol_ = static_cast<int>((worldPos.x - yOffset) / squareSize);
        cellInfo_ = "Row: " + std::to_string(clickedRow_) + ", Col: " + std::to_string(clickedCol_) + "\n" +
                    gameData_.tiles_map[clickedRow_][clickedCol_]->getTileInfo(clickedRow_, clickedCol_);
    }
}
