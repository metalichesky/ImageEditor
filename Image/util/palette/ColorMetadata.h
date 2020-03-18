//
// Created by dmitriy on 16.03.2020.
//

#ifndef IMAGEANALYSER_COLORMETADATA_H
#define IMAGEANALYSER_COLORMETADATA_H


#include "../Color.h"

class ColorMetadata {
public:
    Color *color = nullptr;
    int count = 0;

    ColorMetadata(Color* newColor, int newCount): color(newColor), count(newCount){

    };

    struct FindByColor
    {
        inline bool operator()(const ColorMetadata &one, const ColorMetadata &two) {
            return one.color == two.color;
        }
    };
};


#endif //IMAGEANALYSER_COLORMETADATA_H
