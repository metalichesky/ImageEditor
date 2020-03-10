//
// Created by dmitriy on 09.03.2020.
//

#ifndef IMAGEANALYSER_MAINWINDOW_H
#define IMAGEANALYSER_MAINWINDOW_H


#include "ui/Window.h"
#include "ui/Button.h"
#include "ui/ImageView.h"
#include "util/ImageBMP.h"
#include "util/Listeners.h"
#include "ui/View.h"
#include "util/effects/BlackWhiteEffect.h"
#include "util/effects/FlipEffect.h"
#include "util/effects/RotateEffect.h"
#include "util/effects/FrameEffect.h"
#include <functional>

class MainWindow: public Window {
public:
    Bitmap* currentImageBitmap = nullptr;
    float imageScaleFactor = 0.05;
    ImageView* imageView = new ImageView();
    Button* blackWhiteButton = new Button();
    Button* buttonZoomIn = new Button();
    Button* buttonZoomOut = new Button();
    Button* flipVerticalButton = new Button();
    Button* flipHorizontalButton = new Button();
    Button* rotateRightButton = new Button();
    Button* rotateLeftButton = new Button();
    Button* addFrameButton = new Button();
    Button* cutFrameButton = new Button();

    MainWindow(): Window(){}

    MainWindow(int wWidth, int wHeight, string wTitle):Window(wWidth, wHeight, wTitle) {

    }


    void onCreate() override {
        loadImage();
        imageView->setSize(width, height);
        imageView->setPosition(0,0);
        imageView->loadFromBitmap(currentImageBitmap);
        ui.views.push_back(imageView);

        auto buttonsColor = sf::Color(100,100,100);
        auto buttonsTextColor = sf::Color(0,0,0);

        blackWhiteButton->setSize(150, 50);
        blackWhiteButton->setPosition(0,0);
        blackWhiteButton->setText("Black'n'White");
        blackWhiteButton->setTextColor(buttonsTextColor);
        blackWhiteButton->setBackgroundColor(buttonsColor);
        blackWhiteButton->setTextStyle(sf::Text::Style::Bold);
        blackWhiteButton->setTextSize(18);
        blackWhiteButton->setOnClickListener([this](View *v, sf::Event event) {
            BlackWhiteEffect().applyTo(currentImageBitmap);
            imageView->loadFromBitmap(currentImageBitmap);
        });
        ui.views.push_back(blackWhiteButton);


        flipVerticalButton->setSize(150, 50);
        flipVerticalButton->setPosition(0, 50);
        flipVerticalButton->setText("Flip Vertical");
        flipVerticalButton->setTextColor(buttonsTextColor);
        flipVerticalButton->setBackgroundColor(buttonsColor);
        flipVerticalButton->setTextStyle(sf::Text::Style::Bold);
        flipVerticalButton->setTextSize(18);
        flipVerticalButton->setOnClickListener([this](View *v, sf::Event event) {
            cout << "flipVerticalButton" << endl;
            FlipEffect(Orientation::VERTICAL).applyTo(currentImageBitmap);
            imageView->loadFromBitmap(currentImageBitmap);
        });
        ui.views.push_back(flipVerticalButton);

        flipHorizontalButton->setSize(150, 50);
        flipHorizontalButton->setPosition(0, 100);
        flipHorizontalButton->setText("Flip Horizontal");
        flipHorizontalButton->setTextColor(buttonsTextColor);
        flipHorizontalButton->setBackgroundColor(buttonsColor);
        flipHorizontalButton->setTextStyle(sf::Text::Style::Bold);
        flipHorizontalButton->setTextSize(18);
        flipHorizontalButton->setOnClickListener([this](View *v, sf::Event event) {
            cout << "flipHorizontalButton" << endl;
            FlipEffect(Orientation::HORIZONTAL).applyTo(currentImageBitmap);
            imageView->loadFromBitmap(currentImageBitmap);
        });
        ui.views.push_back(flipHorizontalButton);

        rotateLeftButton->setSize(150, 50);
        rotateLeftButton->setPosition(0, 150);
        rotateLeftButton->setText("Rotate Left");
        rotateLeftButton->setTextColor(buttonsTextColor);
        rotateLeftButton->setBackgroundColor(buttonsColor);
        rotateLeftButton->setTextStyle(sf::Text::Style::Bold);
        rotateLeftButton->setTextSize(18);
        rotateLeftButton->setOnClickListener([this](View *v, sf::Event event) {
            RotateEffect(Direction::LEFT).applyTo(currentImageBitmap);
            imageView->loadFromBitmap(currentImageBitmap);
        });
        ui.views.push_back(rotateLeftButton);

        rotateRightButton->setSize(150, 50);
        rotateRightButton->setPosition(0, 200);
        rotateRightButton->setText("Rotate Right");
        rotateRightButton->setTextColor(buttonsTextColor);
        rotateRightButton->setBackgroundColor(buttonsColor);
        rotateRightButton->setTextStyle(sf::Text::Style::Bold);
        rotateRightButton->setTextSize(18);
        rotateRightButton->setOnClickListener([this](View *v, sf::Event event) {
            RotateEffect(Direction::RIGHT).applyTo(currentImageBitmap);
            imageView->loadFromBitmap(currentImageBitmap);
        });
        ui.views.push_back(rotateRightButton);

        addFrameButton->setSize(150, 50);
        addFrameButton->setPosition(0, 250);
        addFrameButton->setText("Add Frame");
        addFrameButton->setTextColor(buttonsTextColor);
        addFrameButton->setBackgroundColor(buttonsColor);
        addFrameButton->setTextStyle(sf::Text::Style::Bold);
        addFrameButton->setTextSize(18);
        addFrameButton->setOnClickListener([this](View *v, sf::Event event) {
            FrameEffect(1, Color::getRandomColor()).applyTo(currentImageBitmap);
            imageView->loadFromBitmap(currentImageBitmap);
        });
        ui.views.push_back(addFrameButton);

        cutFrameButton->setSize(150, 50);
        cutFrameButton->setPosition(0, 300);
        cutFrameButton->setText("Cut Frame");
        cutFrameButton->setTextColor(buttonsTextColor);
        cutFrameButton->setBackgroundColor(buttonsColor);
        cutFrameButton->setTextStyle(sf::Text::Style::Bold);
        cutFrameButton->setTextSize(18);
        cutFrameButton->setOnClickListener([this](View *v, sf::Event event) {
            FrameEffect(-1, Color::BLACK).applyTo(currentImageBitmap);
            imageView->loadFromBitmap(currentImageBitmap);
        });
        ui.views.push_back(cutFrameButton);

        buttonZoomIn->setSize(100, 100);
        buttonZoomIn->setPosition(700, 0);
        buttonZoomIn->setText("Zoom +");
        buttonZoomIn->setTextColor(buttonsTextColor);
        buttonZoomIn->setBackgroundColor(buttonsColor);
        buttonZoomIn->setTextStyle(sf::Text::Style::Bold);
        buttonZoomIn->setTextSize(18);
        buttonZoomIn->setOnClickListener([this](View *v, sf::Event event) {
            cout << "Lambda callback" << event.text.unicode <<  "\n";
            auto currentScale = imageView->getImageScale();
            currentScale.x += imageScaleFactor;
            currentScale.y += imageScaleFactor;
            imageView->setImageScale(currentScale);
        });
        ui.views.push_back(buttonZoomIn);

        buttonZoomOut->setSize(100, 100);
        buttonZoomOut->setPosition(700 ,100);
        buttonZoomOut->setText("Zoom -");
        buttonZoomOut->setTextColor(buttonsTextColor);
        buttonZoomOut->setBackgroundColor(buttonsColor);
        buttonZoomOut->setTextStyle(sf::Text::Style::Bold);
        buttonZoomOut->setTextSize(18);
        buttonZoomOut->setOnClickListener([this](View *v, sf::Event event) {
            cout << "Lambda callback" << event.text.unicode <<  "\n";
            auto currentScale = imageView->getImageScale();
            currentScale.x -= imageScaleFactor;
            currentScale.y -= imageScaleFactor;
            imageView->setImageScale(currentScale);
        });
        ui.views.push_back(buttonZoomOut);
    }

    void loadImage() {
        string filePath = "/home/dmitriy/University/Image/images/example.bmp";
        ImageBMP image;
        image.readFromFile(filePath);
        currentImageBitmap = image.toBitmap();
        image.clear();
    }

    void onDestroy() override {

    }

    void onStart() override {

    }
};


#endif //IMAGEANALYSER_MAINWINDOW_H
