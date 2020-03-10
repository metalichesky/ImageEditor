//
// Created by dmitriy on 10.03.2020.
//

#ifndef IMAGEANALYSER_BLACKWHITEEFFECT_H
#define IMAGEANALYSER_BLACKWHITEEFFECT_H


#include "../Bitmap.h"
#include "Effect.h"

class BlackWhiteEffect: public Effect{
public:
    void applyTo(Bitmap* bitmap) override{
        int imageWidth = bitmap->width;
        int imageHeight = bitmap->height;
        for (int y = 0; y < imageHeight; y++) {
            for (int x = 0; x < imageWidth; x++) {
                bitmap->getPixel(x, y)->toAvg();
            }
        }
    }
};


#endif //IMAGEANALYSER_BLACKWHITEEFFECT_H
