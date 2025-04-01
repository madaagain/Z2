
#include <iostream>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include "APlayer.hpp"

Player::Player(int id, int y, int x, int orientation, int lvl, std::string teamName, sf::RenderWindow *window)
    : _id(id), _level(lvl), _y(y), _x(x), frameTime_(0.1f), currentFrameTime_(0.f), currentFrame_(0), totalFrames_(0), _team(teamName) {
    setOrientation(orientation);
    createSprite();
    sprite_.setScale(1.5, 1.5);
    rect_ = sf::IntRect(0, 0, 32, 64);
    sprite_.setTextureRect(rect_);
    setPosition(x, y);
    if (window != nullptr)
        window->draw(sprite_);
}

Player::~Player()
{
}

void Player::setOrientation(int orientation)
{
    if (orientation == 1)
        _orientation = Nord;
    if (orientation == 2)
        _orientation = Est;
    if (orientation == 3)
        _orientation = Sud;
    if (orientation == 4)
        _orientation = West;
}

void Player::createSprite() {
    if (_level == 1) {
        if (!texture_.loadFromFile("GUI/dataset/assets/trantorian/TrantorianRightIdle1.png")) {
            std::cerr << "Error loading texture_" << std::endl;
        }
    } else if (_level == 2) {
        if (!texture_.loadFromFile("GUI/dataset/assets/trantorian/TrantorianRightIdle2.png")) {
            std::cerr << "Error loading texture_" << std::endl;
        }
    } else if (_level == 3) {
        if (!texture_.loadFromFile("GUI/dataset/assets/trantorian/TrantorianRightIdle3.png")) {
            std::cerr << "Error loading texture_" << std::endl;
        }
    } else if (_level == 4) {
        if (!texture_.loadFromFile("GUI/dataset/assets/trantorian/TrantorianRightIdle4.png")) {
            std::cerr << "Error loading texture_" << std::endl;
        }
    } else if (_level == 5) {
        if (!texture_.loadFromFile("GUI/dataset/assets/trantorian/TrantorianRightIdle5.png")) {
            std::cerr << "Error loading texture_" << std::endl;
        }
    } else if (_level == 6) {
        if (!texture_.loadFromFile("GUI/dataset/assets/trantorian/TrantorianRightIdle6.png")) {
            std::cerr << "Error loading texture_" << std::endl;
        }
    } else if (_level == 7) {
        if (!texture_.loadFromFile("GUI/dataset/assets/trantorian/TrantorianRightIdle7.png")) {
            std::cerr << "Error loading texture_" << std::endl;
        }
    } else if (_level == 8) {
        if (!texture_.loadFromFile("GUI/dataset/assets/trantorian/TrantorianRightIdle8.png")) {
            std::cerr << "Error loading texture_" << std::endl;
        }
    }
    sprite_.setTexture(texture_);
    sprite_.setScale(sf::Vector2f(1.0, 1.0));
}

void Player::updateAnimation(float deltaTime)
{
    currentFrameTime_ += deltaTime;
    if (currentFrameTime_ >= frameTime_) {
        currentFrame_++;
        if (currentFrame_ >= totalFrames_)
            currentFrame_ = 0;
        currentFrameTime_ = 0;
    }
    rect_.left = currentFrame_ * rect_.width;
    sprite_.setTextureRect(rect_);
}

void Player::draw(sf::RenderWindow &window)
{
    window.draw(sprite_);
}

int Player::getId()
{
    return _id;
}

void Player::setPosition(float x, float y)
{
    sprite_.setPosition(x, y);
}

void Player::setfood(int nbr)
{
    _food = nbr;
};

void  Player::setLinemate(int nbr)
{
    _linemate = nbr;
};

void  Player::setDeraumere(int nbr)
{
    _deraumere = nbr;
};

void  Player::setsibur(int nbr)
{
    _sibur = nbr;
};

void  Player::setmendiane(int nbr)
{
    _mendiane = nbr;
};

void  Player::setphiras(int nbr)
{
    _phiras = nbr;
};

void  Player::setthystame(int nbr)
{
    _thystame = nbr;
};

int Player::getposition()
{
    return _x;
};

void Player::set_level(int i)
{
    _level = i;
}

void Player::dropRessources(int nbr)
{
    if (nbr == 0)
        _food--;
    if (nbr == 1)
        _linemate--;
    if (nbr == 2)
        _deraumere--;
    if (nbr == 3)
        _sibur--;
    if (nbr == 4)
        _mendiane--;
    if (nbr == 5)
        _phiras--;
    if (nbr == 6)
        _thystame--;
}

void Player::CollectRessources(int nbr)
{
    if (nbr == 0)
        _food++;
    if (nbr == 1)
        _linemate++;
    if (nbr == 2)
        _deraumere++;
    if (nbr == 3)
        _sibur++;
    if (nbr == 4)
        _mendiane++;
    if (nbr == 5)
        _phiras++;
    if (nbr == 6)
        _thystame++;
}

void Player::setIncantation()
{
    _incantation = 1;
}
