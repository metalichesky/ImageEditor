//
// Created by dmitriy on 05.03.2020.
//

#ifndef IMAGEANALYSER_BITMAP_H
#define IMAGEANALYSER_BITMAP_H


#include <cstdlib>
#include "Color.h"
#include "effects/Effect.h"

class Bitmap {
public:
    int width = 0;
    int height = 0;

    Bitmap() = default;

    Bitmap(int width, int height) {
        this->width = width;
        this->height = height;
        pixels = new Color[width * height];
    }

    bool setPixel(int x, int y, Color color) {
        bool result = false;
        if (isValidPosition(x, y) && pixels != nullptr) {
            result = true;
            int idx = getPixelIndex(x, y);
            pixels[idx].r = color.r;
            pixels[idx].g = color.g;
            pixels[idx].b = color.b;
            pixels[idx].a = color.a;
        }
        return result;
    }

    bool setPixel(int idx, Color color) {
        bool result = false;
        if (isValidPosition(idx) && pixels != nullptr) {
            result = true;
            pixels[idx].r = color.r;
            pixels[idx].g = color.g;
            pixels[idx].b = color.b;
            pixels[idx].a = color.a;
        }
        return result;
    }

    Color* getPixels() {
        return pixels;
    }


    void setPixels(Color* newPixels){
        pixels = newPixels;
    }

    Color* getPixel(int x, int y) {
        Color* pixel;
        if (isValidPosition(x, y) && pixels != nullptr) {
            pixel = &pixels[getPixelIndex(x, y)];
        }
        return pixel;
    }

    Color* getPixel(int idx) {
        Color* pixel;
        if (isValidPosition(idx) && pixels != nullptr) {
            pixel = &pixels[idx];
        }
        return pixel;
    }

    void clear() {
        if (pixels != nullptr) {
            delete(pixels);
        }
        pixels = nullptr;
        width = 0;
        height = 0;
    }

    ~Bitmap() {
        clear();
    }

private:
    Color *pixels = nullptr;

    int getPixelIndex(int x, int y) {
        return y * width + x;
    }


    bool isValidPosition(int x, int y) {
        bool isValidXPosition = x >= 0 && x < width;
        bool isValidYPosition = y >= 0 && y < height;
        return isValidXPosition && isValidYPosition;
    }

    bool isValidPosition(int idx) {
        return idx >= 0 && idx < width * height;
    }
};


#endif //IMAGEANALYSER_BITMAP_H
