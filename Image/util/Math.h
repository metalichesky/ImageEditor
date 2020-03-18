//
// Created by dmitriy on 18.03.2020.
//

#ifndef IMAGEANALYSER_MATH_H
#define IMAGEANALYSER_MATH_H

#include <algorithm>

using namespace std;

class Math {
public:
    static int clamp(int value, int bottom, int top) {
        return max(bottom, min(value, top));
    }

    static float clamp(float value, float bottom, float top) {
        return max(bottom, min(value, top));
    }
};


#endif //IMAGEANALYSER_MATH_H
