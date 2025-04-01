#ifndef WINDOW_H_
    #define WINDOW_H_
    #include <iostream>
    #include <SFML/Graphics.hpp>

    class Window;
    class GameData;
    class MouseClick;

    #include "Mouse.hpp"
    #include "Egg.hpp"
    #include "Menu.hpp"
    #include "Deraumere.hpp"
    #include "Food.hpp"
    #include "Linemate.hpp"
    #include "Mendiane.hpp"
    #include "Phiras.hpp"
    #include "Sibur.hpp"
    #include "Thysmate.hpp"
    #include "APlayer.hpp"

class Window {
public:
    Window(GameData& gameData);
    ~Window();
    Window() = delete;
    void run(GameData& gameData);
    sf::RenderWindow window_;
    sf::Event event_;
    void windowLoop(GameData& gameData);
    int id;
    float itemPosX;
    float itemPosY;
    bool isOpen();
    GameData &gameData_;
    int windowWidth_;
    int windowHeight_;
    int mapWidth;
    int mapHeight;
    void drawInfoMenu(sf::RenderWindow& window, const std::string& info);
    void completeFullBackground(sf::RenderWindow &window, int squareSize);
    void background_map(int i, int j, sf::RenderWindow &window, int numRows, int numCols, int squareSize, MouseClick& mouse);
    void catchEvent(int numRows, int numCols, GameData& gameData_);
    void checkItem(GameData& gameData_);
    void checkPlayer(GameData& gameData_);
private:
    float deltaTime = 0.0f;
    sf::VideoMode desktop;
    sf::Clock clock;
    int squareSize;
    bool inMenu_ = true;
    std::unique_ptr<Menu> menu_;
    sf::View view;
    sf::Font font;
    sf::Vector2f initialViewOffset;
    sf::Vector2f worldPos;
    ATiles tiles;
    MouseClick mouse;
    std::vector<std::shared_ptr<Deraumere>> deraumereItems_;
    std::vector<std::shared_ptr<Food>> foodItems_;
    std::vector<std::shared_ptr<Linemate>> linemateItems_;
    std::vector<std::shared_ptr<Sibur>> siburItems_;
    std::vector<std::shared_ptr<Mendiane>> mendianeItems_;
    std::vector<std::shared_ptr<Phiras>> phirasItems_;
    std::vector<std::shared_ptr<Thysmate>> thysmateItems_;
    std::vector<std::shared_ptr<Player>> playerItems_;
};

#endif
