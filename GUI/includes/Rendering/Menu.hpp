#ifndef MENU_H_
    #define MENU_H_

#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu(sf::RenderWindow& window);
    bool isPlayButtonClicked(sf::Vector2i mousePos);
    void drawMenu(sf::RenderWindow& window);
    void drawEndGame(sf::RenderWindow& window_);

private:
    sf::Texture backgroundTexture_;
    sf::Sprite backgroundSprite_;
    sf::Texture playButtonTexture_;
    sf::Texture playButtonHoverTexture_;
    sf::Sprite playButtonSprite_;
    sf::Texture endGameTexture;
    sf::Sprite endGameSprite;
    bool isHoveringPlayButton_;
};

#endif