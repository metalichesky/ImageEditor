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
#include <sstream>

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

    uint8_t getComponent(uint componentNum) {
        uint8_t result = 0;
        switch (componentNum) {
            case 0:
                result = r;
                break;
            case 1:
                result = g;
                break;
            case 2:
                result = b;
                break;
            default:
                result = a;
                break;
        }
        return result;
    }

    void setComponent(uint componentNum, uint8_t value) {
        switch (componentNum) {
            case 0:
                r = value;
                break;
            case 1:
                g = value;
                break;
            case 2:
                b = value;
                break;
            default:
                a = value;
                break;
        }
    }

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

    string toString() {
        stringstream ss;
        ss << "r = " << (uint) r << " g = " << (uint) g << " b = " << (uint) b << " a = " << (uint) a;
        return ss.str();
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

    double distanceTo(Color *other, bool useCoefficients = false) {
        double distance = 0;
        if (other == nullptr) return MAXFLOAT;
        distance += pow(r - other->r, 2) * (useCoefficients ? COEF_RED : 1);
        distance += pow(g - other->g, 2) * (useCoefficients ? COEF_GREEN : 1);
        distance += pow(b - other->b, 2) * (useCoefficients ? COEF_BLUE : 1);
//        distance += pow(a - other.a, 2);0
        distance = sqrt(distance);
        return distance;
    }

    long compare(const Color &other) {
        uint32_t sumOne = (r << 16U) | (g << 8U) | b;
        uint32_t sumTwo = (other.r << 16U) | (other.g << 8U) | other.b;
        return (long) (sumOne - sumTwo);
    }

    inline bool operator<(const Color &other) { return compare(other) < 0L; }

    inline bool operator>(const Color &other) { return compare(other) > 0L; }

    inline bool operator>=(const Color &other) { return compare(other) >= 0L; }

    inline bool operator<=(const Color &other) { return compare(other) <= 0L; }

    inline bool operator!=(const Color &other) { return compare(other) != 0L; }

    inline bool operator==(const Color &other) { return compare(other) == 0L; }

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
