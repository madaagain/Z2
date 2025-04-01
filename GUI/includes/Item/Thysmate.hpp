/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** Thysmate.hpp
*/

#ifndef Thysmate_HPP
#define Thysmate_HPP

#include <SFML/Graphics.hpp>

class Thysmate {
public:
    Thysmate(int x, int y, int id, sf::RenderWindow& window);
    ~Thysmate();
    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

private:
    int id_;
    sf::Texture texture;
    sf::Sprite sprite;
};

#endif // Thysmate_HPP
