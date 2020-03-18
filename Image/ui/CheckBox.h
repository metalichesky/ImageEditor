//
// Created by dmitriy on 09.03.2020.
//

#ifndef IMAGEANALYSER_CHECKBOX_H
#define IMAGEANALYSER_CHECKBOX_H

#include <string>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include "View.h"
#include "../util/Color.h"

using namespace std;

class CheckBox: public View {
public:
    CheckBox() {
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

    void setCheckedDrawable(sf::Image* checkedDrawable) {
        this->drawableChecked = checkedDrawable;
        updateCheckedSprite();
    }

    void setUncheckedDrawable(sf::Image* uncheckedDrawable) {
        this->drawableUnchecked = uncheckedDrawable;
        updateCheckedSprite();
    }

    void setCheckedColor(sf::Color checkedColor) {
        this->colorChecked = checkedColor;
        updateCheckedSprite();
    }

    void setUncheckedColor(sf::Color uncheckedColor) {
        this->colorUnchecked = uncheckedColor;
        updateCheckedSprite();
    }

    void setCheckedState(bool newIsChecked) {
        isChecked = newIsChecked;
        updateCheckedSprite();
    }

    bool getCheckedState() {
        return isChecked;
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
        window.draw(checkedSprite);
        window.draw(textObj);
    }

protected:
    bool isChecked = false;
    string text = "";
    sf::Font textFont;
    sf::Text textObj;
    sf::Text::Style textStyle = sf::Text::Style::Regular;
    sf::Image *drawableChecked = nullptr;
    sf::Image *drawableUnchecked = nullptr;
    sf::Color colorChecked = sf::Color::Blue;
    sf::Color colorUnchecked = sf::Color::Transparent;
    sf::Sprite checkedSprite;
    sf::Color textColor = sf::Color::Black;
    int textSize = 12;

    void updateCheckedSprite() {
        auto *backgroundTexture = new sf::Texture();
        auto drawable = isChecked? drawableChecked: drawableUnchecked;
        if (drawable == nullptr) {
            auto color = isChecked? colorChecked : colorUnchecked;
            sf::Image image;
            image.create(size.x, size.y, color);
            backgroundTexture->loadFromImage(image);
        } else {
            backgroundTexture->loadFromImage(*drawable);
            auto textureSize = backgroundTexture->getSize();
            float scaleX = size.x / textureSize.x;
            float scaleY = size.y / textureSize.y;
            backgroundSprite.setScale(scaleX, scaleY);
        }
        checkedSprite.setTexture(*backgroundTexture);
        checkedSprite.setPosition(position.x, position.y);
    }

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
        checkedSprite.setPosition(position);

        auto bounds = textObj.getGlobalBounds();
        int textWidth = bounds.width;
        int textHeight = bounds.height;
        float diffX = size.x - textWidth;
        float diffY = size.y - textHeight;
        textObj.setPosition(position.x + diffX/2.0f, position.y + diffY/2.0f);
    }

};


#endif //IMAGEANALYSER_CHECKBOX_H
