//
// Created by dmitriy on 22.03.2020.
//

#ifndef IMAGEANALYSER_IMAGEFILE_H
#define IMAGEANALYSER_IMAGEFILE_H

#include <iostream>
#include "../Color.h"
#include "../Bitmap.h"
#include "ImageFormat.h"

using namespace std;


class ImageFile {
public:
    virtual void writeToFile(string filePath, ImageFormat format) {}
    virtual void readFromFile(string filePath) {}

    void fromBitmap(Bitmap *bitmap) {
        width = bitmap->width;
        height = bitmap->height;
        int pixelsCount = width * height;
        pixels = new Color[pixelsCount];
        for (int i = 0; i < pixelsCount; i++) {
            pixels[i] = bitmap->getPixel(i)->copy();
        }
    }

    Bitmap *toBitmap() {
        auto *bitmap = new Bitmap(width, height);
        if (pixels == nullptr) return bitmap;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int idx = y * width + x;
                bitmap->setPixel(x, y, pixels[idx]);
            }
        }
        return bitmap;
    }

    ~ImageFile(){
        delete[] pixels;
    }

protected:
    Color *pixels = nullptr;
    uint width = 0;
    uint height = 0;
};


#endif //IMAGEANALYSER_IMAGEFILE_H
