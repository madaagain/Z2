/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** Player.hpp
*/

#ifndef MENDIANE_HPP
#define MENDIANE_HPP

#include <SFML/Graphics.hpp>

class Mendiane {
public:
    Mendiane(int x, int y, int id, sf::RenderWindow& window);
    ~Mendiane();
    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

private:
    int id_;
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif // MENDIANE_HPP
