/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** Phiras.hpp
*/

#ifndef PHIRAS_HPP
#define PHIRAS_HPP

#include <SFML/Graphics.hpp>

class Phiras {
public:
    Phiras(int x, int y, int id, sf::RenderWindow& window);
    ~Phiras();
    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

private:
    int id_;
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif // PHIRAS_HPP
