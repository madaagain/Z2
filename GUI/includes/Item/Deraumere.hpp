/*
** EPITECH PROJECT, 2024
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** Deraumere.hpp
*/

#ifndef DERAMERE_HPP
#define DERAMERE_HPP

#include <SFML/Graphics.hpp>
#include "AStone.hpp"

class Deraumere : public AStones
{
    public:
        Deraumere(int x, int y, int id, sf::RenderWindow &window);
        ~Deraumere();
        void draw(sf::RenderWindow& window);
        void setPosition(float x, float y) override;
    private:
        sf::Sprite sprite;
        sf::Texture texture;
        sf::IntRect rect_;
        int id_;
        void createSprite();
        sf::Vector2f getPosition() const;
        void update() override;
};

#endif // DERAUMERE_HPP
