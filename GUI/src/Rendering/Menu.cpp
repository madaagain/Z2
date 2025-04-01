/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** GUI.hpp
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include "Menu.hpp"

void setMenuScaleDependOnWindowSize(sf::RenderWindow& window, sf::Sprite& sprite)
{
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = sprite.getTexture()->getSize();
    sprite.setScale(static_cast<float>(windowSize.x) / textureSize.x, static_cast<float>(windowSize.y) / textureSize.y);
}

Menu::Menu(sf::RenderWindow& window)
{
    if(!backgroundTexture_.loadFromFile("GUI/dataset/assets/menu/menu_background.png"))
        std::cerr << "Error loading texture" << std::endl;
    if (!playButtonTexture_.loadFromFile("GUI/dataset/assets/menu/play_button_hover.png"))
        std::cerr << "Error loading texture" << std::endl;
    if (!playButtonHoverTexture_.loadFromFile("GUI/dataset/assets/menu/play_button_idle.png"))
        std::cerr << "Error loading texture" << std::endl;
    if(!endGameTexture.loadFromFile("GUI/dataset/assets/menu/end_game.png"))
        std::cerr << "Error loading texture" << std::endl;

    backgroundSprite_.setTexture(backgroundTexture_);
    playButtonSprite_.setTexture(playButtonTexture_);
    setMenuScaleDependOnWindowSize(window, backgroundSprite_);
    endGameSprite.setTexture(endGameTexture);
    setMenuScaleDependOnWindowSize(window, endGameSprite);
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u buttonSize = playButtonTexture_.getSize();
    playButtonSprite_.setPosition((windowSize.x - buttonSize.x) / 2, (windowSize.y - buttonSize.y) / 2);
}

bool Menu::isPlayButtonClicked(sf::Vector2i mousePos)
{
    return playButtonSprite_.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
}

void Menu::drawMenu(sf::RenderWindow& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (playButtonSprite_.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        playButtonSprite_.setTexture(playButtonHoverTexture_);
        isHoveringPlayButton_ = true;
    } else {
        playButtonSprite_.setTexture(playButtonTexture_);
        isHoveringPlayButton_ = false;
    }

    window.draw(backgroundSprite_);
    window.draw(playButtonSprite_);
}

void Menu::drawEndGame(sf::RenderWindow& window_)
{
    window_.draw(endGameSprite);
}
