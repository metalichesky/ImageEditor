//
// Created by dmitriy on 17.03.2020.
//

#include "MouseEvent.h"

uint MouseEvent::TypeMask::NONE = 0b0;
uint MouseEvent::TypeMask::PRESS = 0b1;
uint MouseEvent::TypeMask::RELEASE = 0b10;
uint MouseEvent::TypeMask::MOVE = 0b100;
uint MouseEvent::TypeMask::SCROLL = 0b1000;