#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>

class TextBox {
public:
    TextBox(int size, sf::Color color, bool selected);
    void setFont(sf::Font &font);
    void setPosition(sf::Vector2f pos);
    void setLimit(bool hasLimit);
    void setLimit(bool hasLimit, int limit);
    void setSelected(bool selected);
    std::string getText();
    void draw(sf::RenderWindow &window);
    void typedOn(sf::Event input);
    void deleteLastChar();

private:
    sf::Text text_;
    std::ostringstream textStream_;
    bool isSelected_;
    bool hasLimit_;
    int limit_;
    void inputLogic(int charTyped);
};
