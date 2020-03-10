//
// Created by dmitriy on 09.03.2020.
//

#include "Color.h"
#include <random>
#include <chrono>

using namespace std;

Color::Color(int r, int g, int b, int a): Color(r, g, b) {
    this->a = static_cast<uint8_t>(a);
}

Color::Color(int r, int g, int b) {
    this->r = static_cast<uint8_t>(r);
    this->g = static_cast<uint8_t>(g);
    this->b = static_cast<uint8_t>(b);
}

void Color::toAvg() {
    unsigned int medium = (r + g + b) / 3;
    r = medium;
    g = medium;
    b = medium;
}

Color Color::getColor(int r, int g, int b) {
    Color color(r, g, b);
    return color;
}

Color Color::getColor(int r, int g, int b, int a) {
    Color color(r, g, b, a);
    return color;
}

Color Color::getRandomColor() {
    auto ms = (unsigned int) chrono::system_clock::now().time_since_epoch().count();
    int r = rand_r(&ms) % 256;
    int g = rand_r(&ms) % 256;
    int b = rand_r(&ms) % 256;
    Color color(r, g, b);
    return color;
}

Color Color::BLACK = Color::getColor(0,0,0);

#include "Color.h"
