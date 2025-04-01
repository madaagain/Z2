/*
** EPITECH PROJECT, 2024
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** Linemate.hpp
*/

#include "Linemate.hpp"
#include <iostream>

Linemate::Linemate(int x, int y, int id, sf::RenderWindow& window)
{
    if (!texture.loadFromFile("GUI/dataset/assets/gems/linemate.png")) {
        std::cerr << "Error loading texture" << std::endl;
    }
    id_ = id;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(1.3, 1.3));
    sprite.setPosition(x, y);
    draw(window);
}

Linemate::~Linemate()
{
}

void Linemate::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Linemate::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

sf::Vector2f Linemate::getPosition() const
{
    return sprite.getPosition();
}
