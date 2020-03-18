//
// Created by dmitriy on 16.03.2020.
//

#ifndef IMAGEANALYSER_QUANTIZATION_H
#define IMAGEANALYSER_QUANTIZATION_H


#include "../Color.h"
#include "ColorMetadata.h"
#include <vector>

using namespace std;

class Quantization {
public:
    virtual vector<Color>* quantizeColors(vector<ColorMetadata> *colorsData, int destinationColorsCount) {}
};


#endif //IMAGEANALYSER_QUANTIZATION_H
