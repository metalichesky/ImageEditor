//
// Created by dmitriy on 09.03.2020.
//

#ifndef IMAGEANALYSER_WINDOW_H
#define IMAGEANALYSER_WINDOW_H

#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>
#include "UI.h"
#include "../util/events/MouseEventProcessor.h"

using namespace std;

class Window {
public:
    int width = 800;
    int height = 600;
    string title = "Title";
    sf::Color clearColor = sf::Color::Black;
    UI ui;

    sf::RenderWindow *window;
    sf::Thread *uiThread;

    Window() = default;

    Window(int wWidth, int wHeight, string wTitle) {
        height = wHeight;
        width = wWidth;
        title = wTitle;
    }

    void uiThreadFunc() {
        window = new sf::RenderWindow(sf::VideoMode(width, height), title);
        sf::Event event;
        MouseEventProcessor mouseEventProcessor;
        MouseEvent mouseEvent;
        onStart();
        window->setFramerateLimit(60);
        window->setVerticalSyncEnabled(true);
        while (window->isOpen()) {
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window->close();
                mouseEvent = mouseEventProcessor.process(window, event);
                if (mouseEvent.type != MouseEvent::TypeMask::NONE) {
                    for (int i = ui.views.size()-1; i >= 0; i--) {
                        if (!ui.views[i]->processMouseEvent(mouseEvent)) break;
                    }
                }
            }
            window->clear(clearColor);
            for (View* view: ui.views) {
                view->draw(*window);
            }
            window->display();
        }
        onDestroy();
    }

    void start() {
//        uiThread = new sf::Thread(&Window::uiThreadFunc, this);
        onCreate();
        uiThreadFunc();
    }

    void stop() {
        window->close();
//        uiThread->terminate();
    }

    virtual void onCreate(){

    };

    virtual void onDestroy(){

    };

    virtual void onStart(){

    };
};


#endif //IMAGEANALYSER_WINDOW_H
