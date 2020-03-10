//
// Created by dmitriy on 10.03.2020.
//

#ifndef IMAGEANALYSER_FLIPEFFECT_H
#define IMAGEANALYSER_FLIPEFFECT_H


#include "Effect.h"

enum Orientation{
    VERTICAL, HORIZONTAL
};

class FlipEffect: public Effect {
public:
    enum Orientation direction = Orientation::VERTICAL;

    FlipEffect() = default;

    FlipEffect(Orientation flipDirection) {
        direction = flipDirection;
    }

    void applyTo(Bitmap* bitmap) override {
        if (direction == Orientation::VERTICAL) {
            flipVertical(bitmap);
        } else if (direction == Orientation::HORIZONTAL) {
            flipHorizontal(bitmap);
        }
    }


    void flipHorizontal(Bitmap* bitmap) {
        int imageWidth = bitmap->width;
        int imageHeight = bitmap->height;
        Color currentPixel;
        Color swapPixel;
        for (int y = 0; y < imageHeight; y++) {
            for (int x = 0; x < imageWidth / 2; x++) {
                currentPixel = bitmap->getPixel(x, y)->copy();
                swapPixel = bitmap->getPixel(imageWidth - x - 1, y)->copy();
                bitmap->setPixel(x, y, swapPixel);
                bitmap->setPixel(imageWidth - x - 1, y, currentPixel);
            }
        }
    }

    void flipVertical(Bitmap* bitmap) {
        int imageWidth = bitmap->width;
        int imageHeight = bitmap->height;
        Color currentPixel;
        Color swapPixel;
        for (int y = 0; y < imageHeight / 2; y++) {
            for (int x = 0; x < imageWidth; x++) {
                currentPixel = bitmap->getPixel(x, y)->copy();
                swapPixel = bitmap->getPixel(x, imageHeight - y - 1)->copy();
                bitmap->setPixel(x, y, swapPixel);
                bitmap->setPixel(x, imageHeight - y - 1, currentPixel);
            }
        }
    }
};



#endif //IMAGEANALYSER_FLIPEFFECT_H
