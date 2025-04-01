/*
** EPITECH PROJECT, 2024
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** Player.hpp
*/

#ifndef AITEMS_H_
    #define AITEMS_H_
    #include <string>
    #include <SFML/Graphics.hpp>
    #include "IEntity.hpp"

class AItems : public IEntity
{
    public:
        AItems();
        virtual ~AItems(); /** @brief: Destructeur virtuel pour le polymorphism*/
        void drawSprite() override;
        void drawWindow(sf::RenderWindow& window) override;
        void setPosition(float x, float y) override;
        sf::Vector2f getPosition() override;
        void update() override;

    protected:
        sf::Sprite sprite;
        sf::Texture texture;

    private:

};

#endif /* AITEMS.HPP */