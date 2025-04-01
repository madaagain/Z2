

#include "Sibur.hpp"
#include <iostream>

Sibur::Sibur(int x, int y, int id, sf::RenderWindow& window)
{
    if (!texture.loadFromFile("GUI/dataset/assets/gems/sibur.png")) {
        std::cerr << "Error loading texture" << std::endl;
    }
    id_ = id;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(1.3, 1.3));
    sprite.setPosition(x, y);
    draw(window);
}

Sibur::~Sibur()
{
}

void Sibur::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Sibur::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

sf::Vector2f Sibur::getPosition() const
{
    return sprite.getPosition();
}
