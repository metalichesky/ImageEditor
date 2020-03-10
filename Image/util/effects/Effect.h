//
// Created by dmitriy on 10.03.2020.
//

#ifndef IMAGEANALYSER_EFFECT_H
#define IMAGEANALYSER_EFFECT_H

#include "../Bitmap.h"

class Bitmap;

class Effect {
public:
    virtual void applyTo(Bitmap* bitmap){

    };
};


#endif //IMAGEANALYSER_EFFECT_H
