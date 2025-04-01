

#include "Thysmate.hpp"
#include <iostream>

Thysmate::Thysmate(int x, int y, int id, sf::RenderWindow& window)
{
    if (!texture.loadFromFile("GUI/dataset/assets/gems/thystame.png")) {
        std::cerr << "Error loading texture" << std::endl;
    }
    id_ = id;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(1.3, 1.3));
    sprite.setPosition(x, y);
    draw(window);
}

Thysmate::~Thysmate()
{
}

void Thysmate::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Thysmate::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

sf::Vector2f Thysmate::getPosition() const
{
    return sprite.getPosition();
}
