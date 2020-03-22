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
    virtual void fromBitmap(Bitmap *bitmap) {};
    virtual Bitmap* toBitmap() {}
    virtual void writeToFile(string filePath, ImageFormat format) {}
    virtual void readFromFile(string filePath) {}

    ~ImageFile(){
        delete[] pixels;
    }

protected:
    Color *pixels = nullptr;
    uint width = 0;
    uint height = 0;
};


#endif //IMAGEANALYSER_IMAGEFILE_H
