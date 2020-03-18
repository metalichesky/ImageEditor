//
// Created by dmitriy on 09.03.2020.
//

#ifndef IMAGEANALYSER_COLOR_H
#define IMAGEANALYSER_COLOR_H

#include <cstdint>
#include <algorithm>
#include <SFML/Graphics/Color.hpp>
#include <cmath>
#include <iostream>

using namespace std;

class Color {
public:
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;

    Color(int r, int g, int b);

    Color(int r, int g, int b, int a);

    Color() = default;

    void toAvg();

    static Color BLACK;

    static Color getColor(int r, int g, int b);

    static Color getColor(int r, int g, int b, int a);

    static Color getRandomColor();

    static double COEF_RED;
    static double COEF_GREEN;
    static double COEF_BLUE;

    sf::Color toSFMLColor() {
        return sf::Color(r, g, b, a);
    }

    Color copy() {
        return Color(r, g, b, a);
    }

    int const componentsSum(bool useCoefficients = false) {
        return r + g + b;
    }

//    int componentsSum(bool needAlpha) {
//        return r + g + b + (needAlpha ? a : 0);
//    }

    double distanceTo(Color &other, bool useCoefficients = false) {
        double distance = 0;
        distance += pow(r - other.r, 2) * (useCoefficients? COEF_RED : 1);
        distance += pow(g - other.g, 2) * (useCoefficients? COEF_GREEN : 1);
        distance += pow(b - other.b, 2) * (useCoefficients? COEF_BLUE : 1);
//        distance += pow(a - other.a, 2);0
        distance = sqrt(distance);
        return distance;
    }

    int compare(const Color &other) {
        int result = 0;
        ulong sumOne = (r << 16) + (g << 8) + b;
        ulong sumTwo = (other.r << 16) + (other.g << 8) + other.b;
        return (int) (sumOne - sumTwo);
    }

    inline bool operator<(const Color &other) { return compare(other) < 0; }

    inline bool operator>(const Color &other) { return compare(other) > 0; }

    inline bool operator>=(const Color &other) { return compare(other) >= 0; }

    inline bool operator<=(const Color &other) { return compare(other) <= 0; }

    inline bool operator!=(const Color &other) { return compare(other) != 0; }

    inline bool operator==(const Color &other) { return compare(other) == 0; }

    Color operator+(Color &other) {
        Color newColor = this->copy();
        newColor.r += other.r;
        newColor.g += other.g;
        newColor.b += other.b;
        newColor.a += other.a;
        return newColor;
    }

    Color clamp() {
        Color newColor = this->copy();
        newColor.r = min((int) newColor.r, 255);
        newColor.g = min((int) newColor.g, 255);
        newColor.b = min((int) newColor.b, 255);
        newColor.a = min((int) newColor.a, 255);
        return newColor;
    }

    Color operator-(Color &other) {
        Color newColor = this->copy();
        newColor.r = max(newColor.r - other.r, 0);
        newColor.g = max(newColor.g - other.g, 0);
        newColor.b = max(newColor.b - other.b, 0);
        newColor.a = max(newColor.a - other.a, 0);
        return newColor;
    }

    Color operator/(int num) {
        Color newColor = this->copy();
        if (num > 0) {
            newColor.r /= num;
            newColor.g /= num;
            newColor.b /= num;
            newColor.a /= num;
        }
        return newColor;
    }

    void print() {
        cout << "r = " << (int) r
             << ", g = " << (int) g
             << ", b = " << (int) b
             << ", a = " << (int) a
             << endl;
    }
};

#endif //IMAGEANALYSER_COLOR_H
