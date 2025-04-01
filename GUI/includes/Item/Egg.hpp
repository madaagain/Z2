/*
** EPITECH PROJECT, 2024
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** Player.hpp
*/

#ifndef EGG_H_
    #define EGG_H_

#include <string>
#include <SFML/Graphics.hpp>
#include "AItems.hpp"

class Egg : AItems
{
    public:
        Egg(int x, int y, int id, sf::RenderWindow &window);
        ~Egg();
        enum EggMode { HATCHING, HATCHED } mode_;
        void draw(sf::RenderWindow& window);
        void setPosition(float x, float y) override;
        void hatchAnimation();
    private:
        sf::Sprite sprite;
        sf::Texture texture;
        sf::IntRect rect_;
        int currentFrame_;
        sf::Clock animationClock_;
        float frameDuration_;
        int eggId_;
        void setSpriteRect(sf::IntRect rect);
        void updateAnimation();
        void createSprite();
        sf::Vector2f getPosition() const;
        void update() override;
};

#endif /* FOOD.HPP */
