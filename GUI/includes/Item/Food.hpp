/*
** EPITECH PROJECT, 2024
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** Player.hpp
*/

#ifndef FOOD_H_
    #define FOOD_H_

#include <string>
#include <SFML/Graphics.hpp>
#include "AItems.hpp"

class Food {
public:
    Food(int x, int y, int id, sf::RenderWindow& window);
    ~Food();
    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

private:
    int id_;
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif // FOOD_HPP
