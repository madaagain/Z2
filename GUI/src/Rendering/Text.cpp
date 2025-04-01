/*
** EPITECH PROJECT, 2023
** B-YEP-400-PAR-4-1-zappy-alexis.mer
** File description:
** GUI.hpp
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include "Text.hpp"

TextBox::TextBox(int size, sf::Color color, bool selected) {
    text_.setCharacterSize(size);
    text_.setFillColor(color);
    isSelected_ = selected;
    if (selected) {
        text_.setString("_");
    } else {
        text_.setString("");
    }
    hasLimit_ = false;
    limit_ = 0;
}

void TextBox::setFont(sf::Font &font) {
    text_.setFont(font);
}

void TextBox::setPosition(sf::Vector2f pos) {
    text_.setPosition(pos);
}

void TextBox::setLimit(bool hasLimit) {
    hasLimit_ = hasLimit;
}

void TextBox::setLimit(bool hasLimit, int limit) {
    hasLimit_ = hasLimit;
    limit_ = limit;
}

void TextBox::setSelected(bool selected) {
    isSelected_ = selected;
    if (!selected) {
        std::string t = textStream_.str();
        text_.setString(t);
    }
}

std::string TextBox::getText() {
    return textStream_.str();
}

void TextBox::draw(sf::RenderWindow &window) {
    window.draw(text_);
}

void TextBox::typedOn(sf::Event input) {
    if (isSelected_) {
        int charTyped = input.text.unicode;
        if (charTyped < 128) {
            if (charTyped == 8) {
                deleteLastChar();
            } else {
                inputLogic(charTyped);
            }
        }
    }
}

void TextBox::deleteLastChar() {
    std::string t = textStream_.str();
    std::string newT = "";
    for (size_t i = 0; i < t.length() - 1; i++) {
        newT += t[i];
    }
    textStream_.str("");
    textStream_ << newT;
    text_.setString(textStream_.str() + "_");
}

void TextBox::inputLogic(int charTyped) {
    if (hasLimit_) {
        if (textStream_.str().length() <= limit_) {
            textStream_ << static_cast<char>(charTyped);
        }
    } else {
        textStream_ << static_cast<char>(charTyped);
    }
    text_.setString(textStream_.str() + "_");
}
