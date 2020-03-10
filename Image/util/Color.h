//
// Created by dmitriy on 09.03.2020.
//

#ifndef IMAGEANALYSER_COLOR_H
#define IMAGEANALYSER_COLOR_H

#include <cstdint>
#include <SFML/Graphics/Color.hpp>

class Color {
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a = 255;

    Color(int r, int g, int b);
    Color(int r, int g, int b, int a);

    Color() = default;

    void toAvg();

    static Color BLACK;
    static Color getColor(int r, int g, int b);
    static Color getColor(int r, int g, int b, int a);
    static Color getRandomColor();

    sf::Color toSFMLColor() {
        return sf::Color(r,g,b,a);
    }

    Color copy(){
        return Color(r, g, b, a);
    }
};

#endif //IMAGEANALYSER_COLOR_H
