//
// Created by dmitriy on 10.03.2020.
//

#ifndef IMAGEANALYSER_FRAMEEFFECT_H
#define IMAGEANALYSER_FRAMEEFFECT_H


#include "Effect.h"

class FrameEffect : public Effect {
public:
    int frameWidth = 1;
    Color frameColor = Color::BLACK;

    FrameEffect() = default;

    FrameEffect(int newFrameWidth, Color newFrameColor) {
        frameWidth = newFrameWidth;
        frameColor = newFrameColor;
    }

    void applyTo(Bitmap *bitmap) {
        int newWidth = frameWidth * 2 + bitmap->width;
        int newHeight = frameWidth * 2 + bitmap->height;
        Color *newPixels = new Color[newWidth * newHeight];

        if (frameWidth >= 0) {
            addFrame(newPixels, newWidth, newHeight);
            int idx = 0;
            for (int y = 0; y < bitmap->height; y++) {
                for (int x = 0; x < bitmap->width; x++) {
                    idx = newWidth * (y + frameWidth) + x + frameWidth;
                    newPixels[idx] = *bitmap->getPixel(x, y);
                }
            }
        } else {
            int idx = 0;
            int absFrameWidth = abs(frameWidth);
            for (int y = 0; y < newHeight; y++) {
                for (int x = 0; x < newWidth; x++) {
                    idx = newWidth * y + x;
                    newPixels[idx] = *bitmap->getPixel(x + absFrameWidth, y + absFrameWidth);
                }
            }
        }
        bitmap->width = newWidth;
        bitmap->height = newHeight;
        bitmap->setPixels(newPixels);
    }

private:
    void addFrame(Color *pixels, int width, int height) {
        int x = 0, y = 0;
        int idx = 0;
        //top part
        for (y = 0; y < frameWidth; y++) {
            for (x = 0; x < width; x++) {
                idx = width * y + x;
                pixels[idx] = frameColor;
            }
        }
        //bottom part
        for (y = height - frameWidth - 1; y < height; y++) {
            for (x = 0; x < width; x++) {
                idx = width * y + x;
                pixels[idx] = frameColor;
            }
        }
        //left part
        for (y = 0; y < height; y++) {
            for (x = 0; x < frameWidth; x++) {
                idx = width * y + x;
                pixels[idx] = frameColor;
            }
        }
        //right part
        for (y = 0; y < height; y++) {
            for (x = width - frameWidth - 1; x < width; x++) {
                idx = width * y + x;
                pixels[idx] = frameColor;
            }
        }
    }

};


#endif //IMAGEANALYSER_FRAMEEFFECT_H
