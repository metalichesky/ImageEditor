//
// Created by dmitriy on 09.03.2020.
//

#ifndef IMAGEANALYSER_MAINWINDOW_H
#define IMAGEANALYSER_MAINWINDOW_H


#include "ui/Window.h"
#include "ui/Button.h"
#include "ui/CheckBox.h"
#include "ui/ImageView.h"
#include "util/ImageBMP.h"
#include "util/Listeners.h"
#include "ui/View.h"
#include "util/effects/BlackWhiteEffect.h"
#include "util/effects/FlipEffect.h"
#include "util/effects/RotateEffect.h"
#include "util/effects/FrameEffect.h"
#include "util/effects/ScaleEffect.h"
#include "util/effects/LogoEffect.h"
#include <functional>

class MainWindow: public Window {
public:
    Bitmap* currentImageBitmap = nullptr;
    Bitmap* currentImageLogo = nullptr;
    float imageScaleFactor = 0.05;
    ImageView* imageView = new ImageView();
    ImageView* logoImageView = new ImageView();
    Button* blackWhiteButton = new Button();
    Button* buttonLoadLogo = new Button();
    Button* flipVerticalButton = new Button();
    Button* flipHorizontalButton = new Button();
    Button* rotateRightButton = new Button();
    Button* rotateLeftButton = new Button();
    Button* addFrameButton = new Button();
    Button* cutFrameButton = new Button();
    Button* scaleInButton = new Button();
    Button* scaleOutButton = new Button();
    Button* saveImageButton = new Button();

    CheckBox* checkBoxImage4Button = new CheckBox();
    CheckBox* checkBoxImage8Button = new CheckBox();
    CheckBox* checkBoxImage24Button = new CheckBox();

    MainWindow(): Window(){}

    MainWindow(int wWidth, int wHeight, string wTitle):Window(wWidth, wHeight, wTitle) {

    }


    void onCreate() override {
        string fileName = "cat256.bmp";
//        cout << "Enter file name: ";
//        cin >> fileName;
        loadImage(fileName);
        imageView->setBackgroundColor(sf::Color::Black);
        imageView->setSize(width, height);
        imageView->setPosition(0,0);
        imageView->setImage(currentImageBitmap);
        imageView->setOnScrollListener([this](View *v, MouseEvent event) {
            auto currentScale = imageView->getImageScale();
            currentScale.x += imageScaleFactor * ((event.dScroll < 0)? -1.0f : 1.0f);
            currentScale.y += imageScaleFactor * ((event.dScroll < 0)? -1.0f : 1.0f);
            imageView->setImageScale(currentScale);
            logoImageView->setImageScale(currentScale);
        });
        ui.views.push_back(imageView);

        logoImageView->setBackgroundColor(sf::Color::Transparent);
        logoImageView->setSize(0, 0);
        logoImageView->setPosition(0,0);
        logoImageView->setImage(currentImageBitmap);
        logoImageView->setIsDraggable(true);
        ui.views.push_back(logoImageView);

        auto buttonsColor = sf::Color(100,100,100, 100);
        auto buttonsTextColor = sf::Color::White;

        blackWhiteButton->setSize(150, 50);
        blackWhiteButton->setPosition(0,0);
        blackWhiteButton->setText("Black'n'White");
        blackWhiteButton->setTextColor(buttonsTextColor);
        blackWhiteButton->setBackgroundColor(buttonsColor);
        blackWhiteButton->setTextStyle(sf::Text::Style::Bold);
        blackWhiteButton->setTextSize(18);
        blackWhiteButton->setOnClickListener([this](View *v, MouseEvent event) {
            BlackWhiteEffect().applyTo(currentImageBitmap);
            imageView->setImage(currentImageBitmap);
        });
        ui.views.push_back(blackWhiteButton);


        flipVerticalButton->setSize(150, 50);
        flipVerticalButton->setPosition(0, 50);
        flipVerticalButton->setText("Flip Vertical");
        flipVerticalButton->setTextColor(buttonsTextColor);
        flipVerticalButton->setBackgroundColor(buttonsColor);
        flipVerticalButton->setTextStyle(sf::Text::Style::Bold);
        flipVerticalButton->setTextSize(18);
        flipVerticalButton->setOnClickListener([this](View *v, MouseEvent event) {
            FlipEffect(Orientation::VERTICAL).applyTo(currentImageBitmap);
            imageView->setImage(currentImageBitmap);
        });
        ui.views.push_back(flipVerticalButton);

        flipHorizontalButton->setSize(150, 50);
        flipHorizontalButton->setPosition(0, 100);
        flipHorizontalButton->setText("Flip Horizontal");
        flipHorizontalButton->setTextColor(buttonsTextColor);
        flipHorizontalButton->setBackgroundColor(buttonsColor);
        flipHorizontalButton->setTextStyle(sf::Text::Style::Bold);
        flipHorizontalButton->setTextSize(18);
        flipHorizontalButton->setOnClickListener([this](View *v, MouseEvent event) {
            FlipEffect(Orientation::HORIZONTAL).applyTo(currentImageBitmap);
            imageView->setImage(currentImageBitmap);
        });
        ui.views.push_back(flipHorizontalButton);

        rotateLeftButton->setSize(150, 50);
        rotateLeftButton->setPosition(0, 150);
        rotateLeftButton->setText("Rotate Left");
        rotateLeftButton->setTextColor(buttonsTextColor);
        rotateLeftButton->setBackgroundColor(buttonsColor);
        rotateLeftButton->setTextStyle(sf::Text::Style::Bold);
        rotateLeftButton->setTextSize(18);
        rotateLeftButton->setOnClickListener([this](View *v, MouseEvent event) {
            RotateEffect(Direction::LEFT).applyTo(currentImageBitmap);
            imageView->setImage(currentImageBitmap);
        });
        ui.views.push_back(rotateLeftButton);

        rotateRightButton->setSize(150, 50);
        rotateRightButton->setPosition(0, 200);
        rotateRightButton->setText("Rotate Right");
        rotateRightButton->setTextColor(buttonsTextColor);
        rotateRightButton->setBackgroundColor(buttonsColor);
        rotateRightButton->setTextStyle(sf::Text::Style::Bold);
        rotateRightButton->setTextSize(18);
        rotateRightButton->setOnClickListener([this](View *v, MouseEvent event) {
            RotateEffect(Direction::RIGHT).applyTo(currentImageBitmap);
            imageView->setImage(currentImageBitmap);
        });
        ui.views.push_back(rotateRightButton);

        addFrameButton->setSize(150, 50);
        addFrameButton->setPosition(0, 250);
        addFrameButton->setText("Add Frame");
        addFrameButton->setTextColor(buttonsTextColor);
        addFrameButton->setBackgroundColor(buttonsColor);
        addFrameButton->setTextStyle(sf::Text::Style::Bold);
        addFrameButton->setTextSize(18);
        addFrameButton->setOnClickListener([this](View *v, MouseEvent event) {
            FrameEffect(1, Color::getRandomColor()).applyTo(currentImageBitmap);
            imageView->setImage(currentImageBitmap);
        });
        ui.views.push_back(addFrameButton);

        cutFrameButton->setSize(150, 50);
        cutFrameButton->setPosition(0, 300);
        cutFrameButton->setText("Cut Frame");
        cutFrameButton->setTextColor(buttonsTextColor);
        cutFrameButton->setBackgroundColor(buttonsColor);
        cutFrameButton->setTextStyle(sf::Text::Style::Bold);
        cutFrameButton->setTextSize(18);
        cutFrameButton->setOnClickListener([this](View *v, MouseEvent event) {
            FrameEffect(-1, Color::BLACK).applyTo(currentImageBitmap);
            imageView->setImage(currentImageBitmap);
        });
        ui.views.push_back(cutFrameButton);

        scaleInButton->setSize(150, 50);
        scaleInButton->setPosition(0, 350);
        scaleInButton->setText("Scale +");
        scaleInButton->setTextColor(buttonsTextColor);
        scaleInButton->setBackgroundColor(buttonsColor);
        scaleInButton->setTextStyle(sf::Text::Style::Bold);
        scaleInButton->setTextSize(18);
        scaleInButton->setOnClickListener([this](View *v, MouseEvent event) {
            ScaleEffect(1.5).applyTo(currentImageBitmap);
            imageView->setImage(currentImageBitmap);
        });
        ui.views.push_back(scaleInButton);

        scaleOutButton->setSize(150, 50);
        scaleOutButton->setPosition(0, 400);
        scaleOutButton->setText("Scale -");
        scaleOutButton->setTextColor(buttonsTextColor);
        scaleOutButton->setBackgroundColor(buttonsColor);
        scaleOutButton->setTextStyle(sf::Text::Style::Bold);
        scaleOutButton->setTextSize(18);
        scaleOutButton->setOnClickListener([this](View *v, MouseEvent event) {
            ScaleEffect(0.75).applyTo(currentImageBitmap);
            imageView->setImage(currentImageBitmap);
        });
        ui.views.push_back(scaleOutButton);

        saveImageButton->setSize(150, 50);
        saveImageButton->setPosition(0, 550);
        saveImageButton->setText("Save Image");
        saveImageButton->setTextColor(buttonsTextColor);
        saveImageButton->setBackgroundColor(buttonsColor);
        saveImageButton->setTextStyle(sf::Text::Style::Bold);
        saveImageButton->setTextSize(18);
        saveImageButton->setOnClickListener([this](View *v, MouseEvent event) {
            if (checkBoxImage4Button->getCheckedState()) {
                saveImage("result4bit.bmp", BMPType::COLOR4BIT);
            }
            if (checkBoxImage8Button->getCheckedState()) {
                saveImage("result8bit.bmp",BMPType::COLOR8BIT);
            }
            if (checkBoxImage24Button->getCheckedState()) {
                saveImage("result24bit.bmp",BMPType::COLOR24BIT);
            }
        });
        ui.views.push_back(saveImageButton);

        checkBoxImage4Button->setSize(150, 50);
        checkBoxImage4Button->setPosition(150, 550);
        checkBoxImage4Button->setText("16 Color");
        checkBoxImage4Button->setTextColor(buttonsTextColor);
        checkBoxImage4Button->setBackgroundColor(buttonsColor);
        checkBoxImage4Button->setTextStyle(sf::Text::Style::Bold);
        checkBoxImage4Button->setTextSize(18);
        checkBoxImage4Button->setOnClickListener([this](View *v, MouseEvent event) {
            checkBoxImage4Button->setCheckedState(!checkBoxImage4Button->getCheckedState());
        });
        ui.views.push_back(checkBoxImage4Button);

        checkBoxImage8Button->setSize(150, 50);
        checkBoxImage8Button->setPosition(300, 550);
        checkBoxImage8Button->setText("256 Color");
        checkBoxImage8Button->setTextColor(buttonsTextColor);
        checkBoxImage8Button->setBackgroundColor(buttonsColor);
        checkBoxImage8Button->setTextStyle(sf::Text::Style::Bold);
        checkBoxImage8Button->setTextSize(18);
        checkBoxImage8Button->setOnClickListener([this](View *v, MouseEvent event) {
            checkBoxImage8Button->setCheckedState(!checkBoxImage8Button->getCheckedState());
        });
        ui.views.push_back(checkBoxImage8Button);

        checkBoxImage24Button->setSize(150, 50);
        checkBoxImage24Button->setPosition(450, 550);
        checkBoxImage24Button->setText("True Color");
        checkBoxImage24Button->setTextColor(buttonsTextColor);
        checkBoxImage24Button->setBackgroundColor(buttonsColor);
        checkBoxImage24Button->setTextStyle(sf::Text::Style::Bold);
        checkBoxImage24Button->setTextSize(18);
        checkBoxImage24Button->setOnClickListener([this](View *v, MouseEvent event) {
            checkBoxImage24Button->setCheckedState(!checkBoxImage24Button->getCheckedState());
        });
        ui.views.push_back(checkBoxImage24Button);

        buttonLoadLogo->setSize(100, 100);
        buttonLoadLogo->setPosition(700, 0);
        buttonLoadLogo->setText("Load Logo");
        buttonLoadLogo->setTextColor(buttonsTextColor);
        buttonLoadLogo->setBackgroundColor(buttonsColor);
        buttonLoadLogo->setTextStyle(sf::Text::Style::Bold);
        buttonLoadLogo->setTextSize(18);
        buttonLoadLogo->setOnClickListener([this](View *v, MouseEvent event) {
            loadLogo("logo.bmp");
        });
        ui.views.push_back(buttonLoadLogo);
    }

    void loadImage(string fileName) {
        string filePath = "/home/dmitriy/University/ImageEditor/Image/images/" + fileName;
        ImageBMP image;
        image.readFromFile(filePath);
        currentImageBitmap = image.toBitmap();
    }

    void loadLogo(string fileName) {
        string filePath = "/home/dmitriy/University/ImageEditor/Image/images/" + fileName;
        ImageBMP image;
        image.readFromFile(filePath);
        currentImageLogo = image.toBitmap();
        logoImageView->setImage(currentImageLogo);
        logoImageView->setPosition(width/2-currentImageLogo->width/2, height/2-currentImageLogo->height/2);
        logoImageView->setSize(currentImageLogo->width, currentImageLogo->height);
//        logoImageView->setImageScale(imageView->getImageScale());
    }

    void saveImage(string fileName, BMPType type) {
        string filePath = "/home/dmitriy/University/ImageEditor/Image/images/" + fileName;

        if (currentImageLogo != nullptr) {
            auto logoPositionX = logoImageView->getPosition().x + logoImageView->getImagePosition().x - imageView->getPosition().x - imageView->getImagePosition().x;
            auto logoPositionY = logoImageView->getPosition().y + logoImageView->getImagePosition().y - imageView->getPosition().y - imageView->getImagePosition().y;
            LogoEffect logoEffect = LogoEffect(currentImageLogo, logoPositionX, logoPositionY, 0.5f);
            logoEffect.applyTo(currentImageBitmap);
        }

        ImageBMP image;
        image.fromBitmap(currentImageBitmap);
        image.writeToFile(filePath, type);
    }

    void onDestroy() override {

    }

    void onStart() override {

    }
};


#endif //IMAGEANALYSER_MAINWINDOW_H
