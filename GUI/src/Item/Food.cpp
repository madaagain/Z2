/*
** EPITECH PROJECT, 2024
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** Deraumere.hpp
*/

#include "Food.hpp"
#include <iostream>

Food::Food(int x, int y, int id, sf::RenderWindow& window)
{
    if (!texture.loadFromFile("GUI/dataset/assets/gems/food.png")) {
        std::cerr << "Error loading texture" << std::endl;
    }
    id_ = id;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(1.3, 1.3));
    sprite.setPosition(x, y);
    draw(window);
}

Food::~Food()
{
}

void Food::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Food::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

sf::Vector2f Food::getPosition() const
{
    return sprite.getPosition();
}
