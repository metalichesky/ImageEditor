//
// Created by dmitriy on 17.03.2020.
//

#ifndef IMAGEANALYSER_MOUSEEVENTPROCESSOR_H
#define IMAGEANALYSER_MOUSEEVENTPROCESSOR_H


#include <SFML/Window/Event.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "MouseEvent.h"

class MouseEventProcessor {
    sf::Vector2f oldPosition = sf::Vector2f(0,0);
    sf::Vector2f mousePosition = sf::Vector2f(0,0);
    bool oldPressed = false;
    bool pressed = false;

public:
    MouseEvent process(sf::RenderWindow *window, sf::Event sfmlEvent) {
        mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*((sf::Window*) window)));

        MouseEvent event;
        event.type = MouseEvent::TypeMask::NONE;
        event.x = mousePosition.x;
        event.y = mousePosition.y;
        event.dX = mousePosition.x - oldPosition.x;
        event.dY = mousePosition.y - oldPosition.y;

        if (sfmlEvent.type == sf::Event::MouseButtonPressed) {
            pressed = true;
            switch(sfmlEvent.mouseButton.button) {
                case sf::Mouse::Button::Right : event.button = MouseEvent::MouseButton::RIGHT; break;
                case sf::Mouse::Button::Left : event.button = MouseEvent::MouseButton::LEFT; break;
                case sf::Mouse::Button::Middle : event.button = MouseEvent::MouseButton::WHEEL; break;
            }
            event.type |= MouseEvent::TypeMask::PRESS;
        } else if (sfmlEvent.type == sf::Event::MouseButtonReleased) {
            pressed = false;
            event.type |= MouseEvent::TypeMask::RELEASE;
        }
        if (sfmlEvent.type == sf::Event::MouseWheelMoved) {
            event.type |= MouseEvent::TypeMask::SCROLL;
            event.dScroll = sfmlEvent.mouseWheel.delta;
        }

//
//        if (oldPressed && !pressed) {
//            event.type = MouseEvent::TypeMask::RELEASE;
//        } else if (!oldPressed && pressed) {
//            event.type = MouseEvent::TypeMask::PRESS;
//        }

        if (mousePosition != oldPosition) {
            event.type |= MouseEvent::TypeMask::MOVE;
            if (oldPressed && pressed) {
                //drag
            }
        }

        oldPosition = mousePosition;
        oldPressed = pressed;
        return event;
    }

};


#endif //IMAGEANALYSER_MOUSEEVENTPROCESSOR_H
