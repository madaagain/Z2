/*
** EPITECH PROJECT, 2024
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description: Abstract for different stones
** AStone.hpp
*/

#ifndef ASTONE_H_
    #define ASTONE_H_

#include <string>
#include <SFML/Graphics.hpp>
#include "AItems.hpp"

class AStones : public AItems
{
    public:
    AStones();
    virtual ~AStones() = 0; /** @brief: Destructeur virtuel pour le polymorphism*/
    void drawSprite() override;
    //void setPosition(float x, float y) override;
    //sf::Vector2f getPosition() override;
    //void update() override;

    protected:
        sf::Sprite sprite;
        sf::Texture texture;

};

#endif /* ASTONE.HPP */