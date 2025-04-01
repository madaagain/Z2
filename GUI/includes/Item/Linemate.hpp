/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** Player.hpp
*/

#ifndef LINEMATE_HPP
#define LINEMATE_HPP

#include <SFML/Graphics.hpp>

class Linemate {
public:
    Linemate(int x, int y, int id, sf::RenderWindow& window);
    ~Linemate();
    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

private:
    int id_;
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif // LINEMATE_HPP
