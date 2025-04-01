/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** Sibur.hpp
*/

#ifndef SIBUR_HPP
#define SIBUR_HPP

#include <SFML/Graphics.hpp>

class Sibur {
public:
    Sibur(int x, int y, int id, sf::RenderWindow& window);
    ~Sibur();
    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

private:
    int id_;
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif // SIBUR_HPP
