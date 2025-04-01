/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** GUI.hpp
*/

#include <unistd.h>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Window.hpp"
#include "GameData.hpp"
#include "Menu.hpp"
#include "Mouse.hpp"
#include "Menu.hpp"
#include "ATiles.hpp"
#include "Deraumere.hpp"
#include "Food.hpp"
#include "Linemate.hpp"
#include "Mendiane.hpp"
#include "Phiras.hpp"
#include "Sibur.hpp"
#include "Thysmate.hpp"
#include "APlayer.hpp"

    #define UNUSED(x) (void)(x)

class ATiles;

Window::Window(GameData& gameData) : gameData_(gameData)
{
    desktop = sf::VideoMode::getDesktopMode();
    window_.create(sf::VideoMode(desktop.width, desktop.height), "Zappy");
    window_.setFramerateLimit(60);
    mapWidth = gameData_.getMapWidth();
    mapHeight = gameData_.getMapHeight();
    windowWidth_ = desktop.width;
    windowHeight_ = desktop.height;
    menu_ = std::make_unique<Menu>(window_);
    view = window_.getDefaultView();
    view.setCenter(windowWidth_ / 2, windowHeight_ / 2);
    squareSize = 50;
    mouse.yOffset = (windowWidth_ - (mapWidth * squareSize)) / 2;
    mouse.xOffset = (windowHeight_ - (mapHeight * squareSize)) / 2;
    tiles.windowHeight = windowHeight_;
    tiles.windowWidth = windowWidth_;
    id = 0;
}

Window::~Window() {
    window_.close();
}

void Window::drawInfoMenu(sf::RenderWindow& window, const std::string& info)
{
    if (!font.loadFromFile("GUI/dataset/assets/fonts/fibberish.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }
    sf::View originalView = window.getView();
    window.setView(window.getDefaultView());

    sf::Text text(info, font, 20);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, 10);
    window.draw(text);
    window.setView(originalView);
}

void Window::catchEvent(int numRows, int numCols, GameData& gameData_)
{
    if (event_.type == sf::Event::Closed)
        window_.close();

    if (event_.type == sf::Event::KeyPressed) {
        if (event_.key.code == sf::Keyboard::D &&
            (event_.key.control || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))) {
            window_.close();
        }
    }

    if (event_.type == sf::Event::KeyPressed) {
        if (event_.key.code == sf::Keyboard::Down) {
            view.move(0, 10);
        }
        if (event_.key.code == sf::Keyboard::Up) {
            view.move(0, -10);
        }
        if (event_.key.code == sf::Keyboard::Right) {
            view.move(10, 0);
        }
        if (event_.key.code == sf::Keyboard::Left) {
            view.move(-10, 0);
        }
        if (event_.key.code == sf::Keyboard::Z) {
            view.zoom(0.9);
        }
        if (event_.key.code == sf::Keyboard::S) {
            view.zoom(1.1);
        }
    }

    if (inMenu_) {
        if (event_.type == sf::Event::MouseButtonPressed) {
            if (event_.mouseButton.button == sf::Mouse::Left) {
                if (menu_->isPlayButtonClicked(sf::Mouse::getPosition(window_))) {
                    inMenu_ = false;
                }
            }
        }
    } else {
        if (event_.type == sf::Event::MouseButtonPressed) {
            if (event_.mouseButton.button == sf::Mouse::Left) {
                mouse.handleMouseClick(event_.mouseButton.x, event_.mouseButton.y, numRows, numCols, squareSize, window_, gameData_);
                mouse.dragging = true;
                mouse.initialMousePos = sf::Mouse::getPosition(window_);
                initialViewOffset = view.getCenter();
            }
        }

        if (event_.type == sf::Event::MouseButtonReleased) {
            if (event_.mouseButton.button == sf::Mouse::Left) {
                mouse.dragging = false;
            }
        }

        if (event_.type == sf::Event::MouseMoved && mouse.dragging) {
            mouse.currentMousePos = sf::Mouse::getPosition(window_);
            sf::Vector2f deltaMousePos = sf::Vector2f(mouse.currentMousePos - mouse.initialMousePos);
            view.setCenter(initialViewOffset - deltaMousePos);
        }
    }
}

void Window::checkPlayer(GameData& gameData_)
{
    playerItems_.clear();
    for (int i = 0; i != gameData_.mapWidth; i++) {
        for (int j = 0; j != gameData_.mapHeight; j++) {
            if (gameData_.tiles_map[i][j]->_nbPlayers > 0) {
                for (int k = 0; k != gameData_.tiles_map[i][j]->_nbPlayers; k++) {
                    itemPosX = j * squareSize + mouse.yOffset;
                    itemPosY = i * squareSize + mouse.xOffset - 15;
                    id++;
                    gameData_.tiles_map[i][j]->_Players[k]->setPosition(itemPosX, itemPosY);
                    playerItems_.emplace_back(gameData_.tiles_map[i][j]->_Players[k]);
                }
            }
        }
    }
    for (auto& player : playerItems_) {
        player->updateAnimation(deltaTime);
        player->draw(window_);
    }
}

void Window::checkItem(GameData& gameData_)
{
    deraumereItems_.clear();
    foodItems_.clear();
    linemateItems_.clear();
    siburItems_.clear();
    mendianeItems_.clear();
    phirasItems_.clear();
    thysmateItems_.clear();

    for (int i = 0; i != gameData_.mapWidth; i++) {
        for (int j = 0; j != gameData_.mapHeight; j++) {
            itemPosX = j * squareSize + mouse.yOffset;
            itemPosY = i * squareSize + mouse.xOffset;


            if (gameData_.tiles_map[i][j]->getDeraumere() > 0) {
                id++;
                deraumereItems_.emplace_back(std::make_shared<Deraumere>(itemPosX, itemPosY, id, window_));
            }
            if (gameData_.tiles_map[i][j]->getfood() > 0) {
                id++;
                foodItems_.emplace_back(std::make_shared<Food>(itemPosX, itemPosY + 10, id, window_));
            }
            if (gameData_.tiles_map[i][j]->getLinemate() > 0) {
                id++;
                linemateItems_.emplace_back(std::make_shared<Linemate>(itemPosX, itemPosY + 20, id, window_));
            }
            if (gameData_.tiles_map[i][j]->getSibur() > 0) {
                id++;
                siburItems_.emplace_back(std::make_shared<Sibur>(itemPosX, itemPosY + 30, id, window_));
            }
            if (gameData_.tiles_map[i][j]->getMendiane() > 0) {
                id++;
                mendianeItems_.emplace_back(std::make_shared<Mendiane>(itemPosX + 20, itemPosY, id, window_));
            }
            if (gameData_.tiles_map[i][j]->getPhiras() > 0) {
                id++;
                phirasItems_.emplace_back(std::make_shared<Phiras>(itemPosX + 30, itemPosY, id, window_));
            }
            if (gameData_.tiles_map[i][j]->getThystame() > 0) {
                id++;
                thysmateItems_.emplace_back(std::make_shared<Thysmate>(itemPosX + 20, itemPosY + 20, id, window_));
            }
        }
    }
}

void Window::windowLoop(GameData& gameData_)
{   
    while (window_.pollEvent(event_)) {
        catchEvent(mapWidth, mapHeight, gameData_);
    }

    window_.clear();

    deltaTime = clock.restart().asSeconds();
    if (gameData_.EndOfGame) {
        menu_->drawEndGame(window_);
    } else if (inMenu_) {
        menu_->drawMenu(window_);
    } else {
        window_.setView(view);
        tiles.drawGrid(window_, mapWidth, mapHeight, squareSize, mouse);
        checkItem(gameData_);
        checkPlayer(gameData_);
        if (!mouse.cellInfo_.empty()) {
            drawInfoMenu(window_, mouse.cellInfo_);
        }
    }
    window_.display();
}

void Window::run(GameData& gameData_) {
    windowLoop(gameData_);
}

bool Window::isOpen() {
    return window_.isOpen();
}
