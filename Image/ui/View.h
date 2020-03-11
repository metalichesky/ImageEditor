//
// Created by dmitriy on 09.03.2020.
//

#ifndef IMAGEANALYSER_VIEW_H
#define IMAGEANALYSER_VIEW_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <functional>
#include <SFML/Window/Event.hpp>
#include "../util/Color.h"


using namespace std;

class View {
    typedef function<void(View *, sf::Event)> OnClickListener;
public:
    OnClickListener onClickListener;

    void setOnClickListener(OnClickListener newOnClickListener) {
        this->onClickListener = newOnClickListener;
    }

    bool checkClick(int x, int y) {
        bool inX = (x > position.x) && (x < position.x + size.x);
        bool inY = (y > position.y) && (y < position.y + size.y);
        return inX && inY;
    }

    void processClick(int x, int y, sf::Event event) {
        if (checkClick(x, y)) {
            if (onClickListener != nullptr) {
                (onClickListener)(this, event);
            }
        }
    }

    virtual void draw(sf::RenderWindow &window) {
        window.draw(backgroundSprite);
    }

    void setBackgroundColor(sf::Color color) {
        backgroundColor = color;
        backgroundSprite = sf::Sprite();
        updateBackground();
    }

    void setBackgroundDrawable(sf::Image *image) {
        backgroundDrawable = image;
        backgroundSprite = sf::Sprite();
        updateBackground();
    }

    void setPosition(sf::Vector2f newPosition){
        position = newPosition;
        updateBackground();
    }

    void setPosition(float newPositionX, float newPositionY){
        position.x = newPositionX;
        position.y = newPositionY;
        updateBackground();
    }

    sf::Vector2f getPosition() {
        return position;
    }

    void setSize(sf::Vector2f newSize){
        size = newSize;
        updateBackground();
    }

    void setSize(float newSizeX, float newSizeY){
        size.x = newSizeX;
        size.y = newSizeY;
        updateBackground();
    }

    sf::Vector2f getSize() {
        return size;
    }

protected:
    sf::Vector2f position = sf::Vector2f(0.0f, 0.0f);
    sf::Vector2f size = sf::Vector2f(0.0f, 0.0f);
    sf::Color backgroundColor = sf::Color::White;
    sf::Image *backgroundDrawable = nullptr;
    sf::Sprite backgroundSprite = sf::Sprite();

    void updateBackground() {
        auto *backgroundTexture = new sf::Texture();
        if (backgroundDrawable == nullptr) {
            sf::Image image;
            image.create(size.x, size.y, backgroundColor);
            backgroundTexture->loadFromImage(image);
        } else {
            backgroundTexture->loadFromImage(*backgroundDrawable);
            auto textureSize = backgroundTexture->getSize();
            float scaleX = size.x / textureSize.x;
            float scaleY = size.y / textureSize.y;
            backgroundSprite.setScale(scaleX, scaleY);
        }
        backgroundSprite.setTexture(*backgroundTexture);
        backgroundSprite.setPosition(position.x, position.y);
    }
};


#endif //IMAGEANALYSER_VIEW_H
