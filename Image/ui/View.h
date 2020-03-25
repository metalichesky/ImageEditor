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
#include "../util/events/MouseEvent.h"


using namespace std;

class View {
    typedef function<void(View *, MouseEvent)> OnClickListener;
    typedef function<void(View*, MouseEvent)> OnScrollListener;
public:
    OnClickListener onClickListener;
    OnScrollListener onScrollListener;

    void setOnClickListener(OnClickListener newOnClickListener) {
        this->onClickListener = newOnClickListener;
    }

    void setOnScrollListener(OnScrollListener newOnScrollListener) {
        this->onScrollListener = newOnScrollListener;
    }

    bool checkBorders(float x, float y) {
        bool inX = (x > position.x) && (x < position.x + size.x);
        bool inY = (y > position.y) && (y < position.y + size.y);
        return inX && inY;
    }

    virtual bool processMouseEvent(MouseEvent event) {
        bool allowContinueProcessing = true;
        if (checkBorders(event.x, event.y) && isVisible) {
            if ((event.type & MouseEvent::TypeMask::PRESS) != 0) {
                allowContinueProcessing = false;
                pressed = true;
            }
            if ((event.type & MouseEvent::TypeMask::RELEASE) != 0){
                if (pressed && !dragging) {
                    if (onClickListener != nullptr) {
                        (onClickListener)(this, event);
                    }
                }
            }
            if ((event.type & MouseEvent::TypeMask::SCROLL) != 0) {
                if (onScrollListener != nullptr) {
                    (onScrollListener)(this, event);
                }
                allowContinueProcessing = false;
            }
        }
        if ((event.type & MouseEvent::TypeMask::MOVE) != 0) {
            if (pressed && isDraggable) {
                dragging = true;
                allowContinueProcessing = false;
                drag(event);
            }
        }
        if ((event.type & MouseEvent::TypeMask::RELEASE) != 0){
            dragging = false;
            pressed = false;
        }
        return allowContinueProcessing;
    }

    virtual void draw(sf::RenderWindow &window) {
        if (isVisible) {
            updatePosition();
            window.draw(backgroundSprite);
        }
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

    void setDraggable(bool newIsDraggable) {
        this->isDraggable = newIsDraggable;
    }

    bool getDraggable() {
        return this->isDraggable;
    }

    void setVisible(bool newIsVisible) {
        isVisible = newIsVisible;
    }

    bool getVisible() {
        return isVisible;
    }


protected:
    sf::Vector2f position = sf::Vector2f(0.0f, 0.0f);
    sf::Vector2f size = sf::Vector2f(0.0f, 0.0f);
    sf::Color backgroundColor = sf::Color::White;
    sf::Image *backgroundDrawable = nullptr;
    sf::Sprite backgroundSprite = sf::Sprite();
    bool pressed = false;
    bool dragging = false;
    bool isDraggable = false;
    bool isVisible = true;

    void updateBackground() {
        auto *backgroundTexture = new sf::Texture();
        if (size.x <= 0 && size.y <= 0) return;
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

private:
    void updatePosition() {
        backgroundSprite.setPosition(position);
    }

    void drag(MouseEvent &event) {
        position.x += event.dX;
        position.y += event.dY;
    }

};


#endif //IMAGEANALYSER_VIEW_H
