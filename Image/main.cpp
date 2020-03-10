#include <chrono>
#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include "util/ImageBMP.h"
#include "ui/Window.h"
#include "MainWindow.h"

#define W 500
#define H 500

using namespace std;


void program(sf::Image *buffer, sf::Texture *texture) {
//    readImage("/home/dmitriy/University/Image/images/example.bmp");
    string filePath = "/home/dmitriy/University/Image/images/example2.bmp";
    ImageBMP image;
    image.readFromFile(filePath);
    auto bitmap = image.toBitmap();
    image.clear();
    buffer->create(bitmap->width, bitmap->height, sf::Color::Black);
    int pixelsCounter = 0;
    for (int y = 0; y < bitmap->height; y++) {
        for (int x = 0; x < bitmap->width; x++) {
            auto pixel = bitmap->getPixel(x, y);
            buffer->setPixel(x, y, sf::Color(pixel->r, pixel->g, pixel->b, pixel->a));
            pixelsCounter++;
        }
    }
    texture->create(bitmap->width, bitmap->height);
}

string getCurrentPath(char* argv[]) {
    string argv_str(argv[0]);
    string base = argv_str.substr(0, argv_str.find_last_of("/"));
    cout << base << endl;
}

int main(int argc, char* argv[]) {

//    readImage("/home/dmitriy/University/ImageAnalyser/images/example.bmp");



    MainWindow window(800, 600, "ImageAnalyser");
    window.start();

//
//    auto *buffer = new sf::Image();
//    sf::Texture *texture = new sf::Texture();
//
//    program(buffer, texture);
//
//    buffer.create(windowW, windowH, sf::Color::Black);
//
//
//    sf::Sprite bufferSprite;
//    bufferSprite.setScale(0.5f, 0.5f);


    return 0;
}