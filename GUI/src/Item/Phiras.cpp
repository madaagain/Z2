
#include "Phiras.hpp"
#include <iostream>

Phiras::Phiras(int x, int y, int id, sf::RenderWindow& window)
{
    if (!texture.loadFromFile("GUI/dataset/assets/gems/phiras.png")) {
        std::cerr << "Error loading texture" << std::endl;
    }
    id_ = id;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(1.3, 1.3));
    sprite.setPosition(x, y);
    draw(window);
}

Phiras::~Phiras()
{
}

void Phiras::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Phiras::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

sf::Vector2f Phiras::getPosition() const
{
    return sprite.getPosition();
}
