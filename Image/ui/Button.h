//
// Created by dmitriy on 09.03.2020.
//

#ifndef IMAGEANALYSER_BUTTON_H
#define IMAGEANALYSER_BUTTON_H

#include <string>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include "View.h"
#include "../util/Color.h"

using namespace std;

class Button: public View {
public:
    Button() {
        textFont.loadFromFile("../fonts/GraublauWeb.otf");
    }

    void setText(string newText) {
        text = newText;
        updateText();
    }

    string getText() {
        return text;
    }

    sf::Font setTextFont(sf::Font font) {
        textFont = font;
        updateText();
    }

    sf::Font getTextFont() {
        return textFont;
    }

    void setTextColor(sf::Color newColor) {
        textColor = newColor;
        updateText();
    }

    sf::Color getTextColor() {
        return textColor;
    }

    void setTextStyle(sf::Text::Style newTextStyle) {
        textStyle = newTextStyle;
        updateText();
    }

    sf::Text::Style getTextStyle() {
        return textStyle;
    }

    void setTextSize(int newSize) {
        textSize = newSize;
        updateText();
    }

    int getTextSize() {
        return textSize;
    }

    void draw(sf::RenderWindow &window) override {
        View::draw(window);
        updatePosition();
        window.draw(textObj);
    }

protected:
    string text = "";
    sf::Font textFont;
    sf::Text textObj;
    sf::Text::Style textStyle = sf::Text::Style::Regular;
    sf::Color textColor = sf::Color::Black;
    int textSize = 12;

    void updateText() {
        textObj.setString(text);
        textObj.setFont(textFont);
        textObj.setCharacterSize(textSize);
        textObj.setFillColor(sf::Color(textColor.r, textColor.g, textColor.b, textColor.a));
        textObj.setPosition(position);
        textObj.setStyle(textStyle);
    }
private:
    void updatePosition() {
        auto bounds = textObj.getGlobalBounds();
        int textWidth = bounds.width;
        int textHeight = bounds.height;
        float diffX = size.x - textWidth;
        float diffY = size.y - textHeight;
        textObj.setPosition(position.x + diffX/2.0f, position.y + diffY/2.0f);
    }
};


#endif //IMAGEANALYSER_BUTTON_H
