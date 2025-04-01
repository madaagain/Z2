#ifndef PLAYER_H_
    #define PLAYER_H_

#include <string>
#include <SFML/Graphics.hpp>
#include "AItems.hpp"

class Player : public AItems
{
    public:
        Player() = default;
        Player(int id, int y, int x, int orientation, int lvl, std::string teamName, sf::RenderWindow *window);
        ~Player();
        void setPosition(float x, float y) override;
        void createSprite();
        void setOrientation(int orientation);
        enum Orientation {Nord, Est, Sud, West};
        int getId();
        void setfood(int nbr);
        void setLinemate(int nbr);
        void setDeraumere(int nbr);
        void setsibur(int nbr);
        void setmendiane(int nbr);
        void setphiras(int nbr);
        void setthystame(int nbr);
        int getposition();
        void set_level(int i);
        void dropRessources(int nbr);
        void CollectRessources(int nbr);
        void setIncantation();
        void updateAnimation(float deltaTime);
        void draw(sf::RenderWindow &window);

    private:
        sf::Sprite sprite_;
        sf::Texture texture_;
        sf::IntRect rect_;
        int _id;
        int _level;
        int _y;
        int _x;
        int _food;
        int _linemate;
        int _deraumere;
        int _sibur;
        int _mendiane;
        int _phiras;
        int _thystame;
        int _nbPlayers;
        int _incantation;
        float frameTime_;
        float currentFrameTime_;
        int currentFrame_;
        int totalFrames_;
        std::string _team;
        Orientation _orientation;
};

#endif /* PLAYER.HPP */
