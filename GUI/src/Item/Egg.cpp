/*
** EPITECH PROJECT, 2024
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** main
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Egg.hpp"

Egg::Egg(int x, int y, int id, sf::RenderWindow &window)
{
    createSprite();
    eggId_ = id;
    mode_ = HATCHING;
    frameDuration_ = 0.2f;
    currentFrame_ = 0;
    setPosition(x, y);
    draw(window);
}

Egg::~Egg()
{
}

void Egg::hatchAnimation() {
    updateAnimation();
}

void Egg::createSprite() {
    if (!texture.loadFromFile("GUI/dataset/assets/trantorian/egg.png")) {
        std::cerr << "Error loading texture" << std::endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(1.0, 1.0));
    rect_ = sf::IntRect(0, 0, 40, 39);
    setSpriteRect(rect_);
}

void Egg::setSpriteRect(sf::IntRect rect) {
    sprite.setTextureRect(rect);
}

void Egg::updateAnimation() {
    if (animationClock_.getElapsedTime().asSeconds() > frameDuration_) {
        if (mode_ == HATCHING) {
            if (rect_.left < 80) {
                rect_.left += 40;
            } else if (rect_.left == 80 && rect_.top < 39 * 3) {
                rect_.left = 0;
                rect_.top += 39;
            } else if (rect_.left == 80 && rect_.top == 39 * 3) {
                rect_.left = 80;
                rect_.top = 39 * 3;
                mode_ = HATCHED;
            }
        }
        setSpriteRect(rect_);
        animationClock_.restart();
    }
}

void Egg::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void Egg::update()
{
    updateAnimation();
}

void Egg::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

sf::Vector2f Egg::getPosition() const
{
    return sprite.getPosition();
}
