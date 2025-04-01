/*
** EPITECH PROJECT, 2024
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** Deraumere.hpp
*/

#include "Deraumere.hpp"
#include <errno.h>
#include <iostream>

Deraumere::Deraumere(int x, int y, int id, sf::RenderWindow &window)
{
    if (!texture.loadFromFile("GUI/dataset/assets/gems/first_gem.png")) {
        std::cerr << "Error loading texture" << std::endl;
    }
    id_ = id;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(1.3, 1.3));
    sprite.setPosition(x, y);
    draw(window);
}

Deraumere::~Deraumere()
{
}

void Deraumere::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Deraumere::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

sf::Vector2f Deraumere::getPosition() const
{
    return sprite.getPosition();
}

void Deraumere::update()
{

}
