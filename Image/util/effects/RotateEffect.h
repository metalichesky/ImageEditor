//
// Created by dmitriy on 10.03.2020.
//

#ifndef IMAGEANALYSER_ROTATEEFFECT_H
#define IMAGEANALYSER_ROTATEEFFECT_H

#include "Effect.h"

enum Direction {
    LEFT, RIGHT
};

class RotateEffect : public Effect {
public:
    RotateEffect() = default;
    Direction direction = Direction::LEFT;

    RotateEffect(Direction rotateDirection): RotateEffect() {
        direction = rotateDirection;
    }

    void applyTo(Bitmap *bitmap) override {
        int imageWidth = bitmap->width;
        int imageHeight = bitmap->height;
        int newImageWidth = imageHeight;
        int newImageHeight = imageWidth;
        auto *newPixels = new Color[newImageWidth * newImageHeight];
        int oldIdx = 0;
        int newX = 0;
        int newY = 0;
        int newIdx = 0;
        bool clockwise = direction == Direction::RIGHT;
        Color *currentPixel;
        for (int y = 0; y < imageHeight; y++) {
            for (int x = 0; x < imageWidth; x++) {
                oldIdx = imageWidth * y + x;
                currentPixel = bitmap->getPixel(oldIdx);
                if (clockwise) {
                    newY = x;
                    newX = newImageWidth - y - 1;
                } else {
                    newY = newImageHeight - x - 1;
                    newX = y;
                }
                newIdx = newImageWidth * newY + newX;
                newPixels[newIdx] = *currentPixel;
            }
        }
        bitmap->width = newImageWidth;
        bitmap->height = newImageHeight;
        bitmap->setPixels(newPixels);
    }
};


#endif //IMAGEANALYSER_ROTATEEFFECT_H
