

#include "Mendiane.hpp"
#include <iostream>

Mendiane::Mendiane(int x, int y, int id, sf::RenderWindow& window)
{
    if (!texture.loadFromFile("GUI/dataset/assets/gems/mendiane.png")) {
        std::cerr << "Error loading texture" << std::endl;
    }
    id_ = id;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(1.3, 1.3));
    sprite.setPosition(x, y);
    draw(window);
}

Mendiane::~Mendiane()
{
}

void Mendiane::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Mendiane::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

sf::Vector2f Mendiane::getPosition() const
{
    return sprite.getPosition();
}
