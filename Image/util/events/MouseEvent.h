//
// Created by dmitriy on 17.03.2020.
//

#ifndef IMAGEANALYSER_MOUSEEVENT_H
#define IMAGEANALYSER_MOUSEEVENT_H


#include <sys/types.h>



class MouseEvent {
public:

    enum MouseButton {
        LEFT, RIGHT, WHEEL, NONE
    };

    class TypeMask {
    public:
        static uint NONE;
        static uint PRESS;
        static uint RELEASE;
        static uint MOVE;
        static uint SCROLL;
    };

    float x = 0;
    float y = 0;
    float dX = 0;
    float dY = 0;
    float dScroll = 0;
    uint type = 0;
    MouseButton button = MouseButton::NONE;

};


#endif //IMAGEANALYSER_MOUSEEVENT_H
