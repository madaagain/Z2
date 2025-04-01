/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** GUI.hpp
*/


#ifndef IITEM_H_
    #define IITEM_H_

#include <SFML/Graphics.hpp>

class IEntity {
public:
    virtual ~IEntity() = default;
    virtual void drawSprite() = 0;
    virtual void drawWindow(sf::RenderWindow& window) = 0;
    virtual void setPosition(float x, float y) = 0;
    virtual sf::Vector2f getPosition() = 0;
    virtual void update() = 0;
};

#endif /* GUI.HPP */
