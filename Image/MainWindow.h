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
#include "util/ImagePCX.h"
#include "util/Listeners.h"
#include "ui/View.h"
#include "util/effects/BlackWhiteEffect.h"
#include "util/effects/FlipEffect.h"
#include "util/effects/RotateEffect.h"
#include "util/effects/FrameEffect.h"
#include "util/effects/ScaleEffect.h"
#include "util/effects/LogoEffect.h"
#include "util/crypto/BitmapCoder.h"
#include <functional>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

class MainWindow : public Window {
public:
    Bitmap *currentImageBitmap = nullptr;
    Bitmap *currentLogoBitmap = nullptr;
    float imageScaleFactor = 0.05;
    ImageView *imageView = new ImageView();
    ImageView *logoImageView = new ImageView();
    Button *blackWhiteButton = new Button();
    Button *buttonLoadLogo = new Button();
    Button *buttonPrintLogo = new Button();
    Button *buttonHideLogo = new Button();
    Button *flipVerticalButton = new Button();
    Button *flipHorizontalButton = new Button();
    Button *rotateRightButton = new Button();
    Button *rotateLeftButton = new Button();
    Button *addFrameButton = new Button();
    Button *cutFrameButton = new Button();
    Button *scaleInButton = new Button();
    Button *scaleOutButton = new Button();
    Button *loadImageButton = new Button();
    Button *saveImageButton = new Button();
    Button *uiVisibilityButton = new Button();
    Button *encodeDataButton = new Button();
    Button *decodeDataButton = new Button();

    CheckBox *checkBoxBMP4Button = new CheckBox();
    CheckBox *checkBoxBMP8Button = new CheckBox();
    CheckBox *checkBoxBMP24Button = new CheckBox();

    CheckBox *checkBoxPCX4Button = new CheckBox();
    CheckBox *checkBoxPCX8Button = new CheckBox();
    CheckBox *checkBoxPCX24Button = new CheckBox();

    string currentDir = fs::path(fs::current_path()).string();
    string imageDir = "/images/";

    bool uiVisible = true;

    MainWindow() : Window() {}

    MainWindow(int wWidth, int wHeight, string wTitle) : Window(wWidth, wHeight, wTitle) {

    }

    void onCreate() override {
        loadPCXImage("wolf24.pcx");

        imageView->setBackgroundColor(sf::Color::Black);
        imageView->setSize(width, height);
        imageView->setPosition(0, 0);
        imageView->setImage(currentImageBitmap);
//        imageView->setOnScrollListener([this](View *v, MouseEvent event) {
//            auto currentScale = imageView->getImageScale();
//            currentScale.x += imageScaleFactor * ((event.dScroll < 0)? -1.0f : 1.0f);
//            currentScale.y += imageScaleFactor * ((event.dScroll < 0)? -1.0f : 1.0f);
//            imageView->setImageScale(currentScale);
//            logoImageView->setImageScale(currentScale);
//        });
        imageView->setOnScaledListener([this](View *v, sf::Vector2f scale) {
            logoImageView->setImageScale(scale);
        });
        ui.views.push_back(imageView);

        logoImageView->setBackgroundColor(sf::Color::Transparent);
        logoImageView->setSize(0, 0);
        logoImageView->setPosition(0, 0);
        logoImageView->setImage(currentImageBitmap);
        logoImageView->setDraggable(true);
        logoImageView->setVisible(false);
        ui.views.push_back(logoImageView);

        auto buttonsColor = sf::Color(100, 100, 100, 100);
        auto buttonsTextColor = sf::Color::White;

        blackWhiteButton->setSize(150, 50);
        blackWhiteButton->setPosition(0, 0);
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
        saveImageButton->setPosition(0, 500);
        saveImageButton->setText("Save Image");
        saveImageButton->setTextColor(buttonsTextColor);
        saveImageButton->setBackgroundColor(buttonsColor);
        saveImageButton->setTextStyle(sf::Text::Style::Bold);
        saveImageButton->setTextSize(18);
        saveImageButton->setOnClickListener([this](View *v, MouseEvent event) {
            if (checkBoxBMP4Button->getCheckedState()) {
                saveImage("result4bit.bmp", BMPType::COLOR4BIT);
            }
            if (checkBoxBMP8Button->getCheckedState()) {
                saveImage("result8bit.bmp", BMPType::COLOR8BIT);
            }
            if (checkBoxBMP24Button->getCheckedState()) {
                saveImage("result24bit.bmp", BMPType::COLOR24BIT);
            }
        });
        ui.views.push_back(saveImageButton);

        loadImageButton->setSize(150, 50);
        loadImageButton->setPosition(0, 450);
        loadImageButton->setText("Load Image");
        loadImageButton->setTextColor(buttonsTextColor);
        loadImageButton->setBackgroundColor(buttonsColor);
        loadImageButton->setTextStyle(sf::Text::Style::Bold);
        loadImageButton->setTextSize(18);
        loadImageButton->setOnClickListener([this](View *v, MouseEvent event) {
            string fileName = "";
            cout << "Enter file name: ";
            cin >> fileName;
            loadImage(fileName);
        });
        ui.views.push_back(loadImageButton);

        checkBoxBMP4Button->setSize(150, 50);
        checkBoxBMP4Button->setPosition(150, 500);
        checkBoxBMP4Button->setText("BMP 4bit");
        checkBoxBMP4Button->setTextColor(buttonsTextColor);
        checkBoxBMP4Button->setBackgroundColor(buttonsColor);
        checkBoxBMP4Button->setTextStyle(sf::Text::Style::Bold);
        checkBoxBMP4Button->setTextSize(18);
        checkBoxBMP4Button->setOnClickListener([this](View *v, MouseEvent event) {
            checkBoxBMP4Button->setCheckedState(!checkBoxBMP4Button->getCheckedState());
        });
        ui.views.push_back(checkBoxBMP4Button);

        checkBoxBMP8Button->setSize(150, 50);
        checkBoxBMP8Button->setPosition(300, 500);
        checkBoxBMP8Button->setText("BMP 8bit");
        checkBoxBMP8Button->setTextColor(buttonsTextColor);
        checkBoxBMP8Button->setBackgroundColor(buttonsColor);
        checkBoxBMP8Button->setTextStyle(sf::Text::Style::Bold);
        checkBoxBMP8Button->setTextSize(18);
        checkBoxBMP8Button->setOnClickListener([this](View *v, MouseEvent event) {
            checkBoxBMP8Button->setCheckedState(!checkBoxBMP8Button->getCheckedState());
        });
        ui.views.push_back(checkBoxBMP8Button);

        checkBoxBMP24Button->setSize(150, 50);
        checkBoxBMP24Button->setPosition(450, 500);
        checkBoxBMP24Button->setText("BMP 24bit");
        checkBoxBMP24Button->setTextColor(buttonsTextColor);
        checkBoxBMP24Button->setBackgroundColor(buttonsColor);
        checkBoxBMP24Button->setTextStyle(sf::Text::Style::Bold);
        checkBoxBMP24Button->setTextSize(18);
        checkBoxBMP24Button->setOnClickListener([this](View *v, MouseEvent event) {
            checkBoxBMP24Button->setCheckedState(!checkBoxBMP24Button->getCheckedState());
        });
        ui.views.push_back(checkBoxBMP24Button);

        checkBoxPCX4Button->setSize(150, 50);
        checkBoxPCX4Button->setPosition(150, 550);
        checkBoxPCX4Button->setText("PCX 4bit");
        checkBoxPCX4Button->setTextColor(buttonsTextColor);
        checkBoxPCX4Button->setBackgroundColor(buttonsColor);
        checkBoxPCX4Button->setTextStyle(sf::Text::Style::Bold);
        checkBoxPCX4Button->setTextSize(18);
        checkBoxPCX4Button->setOnClickListener([this](View *v, MouseEvent event) {
            checkBoxPCX4Button->setCheckedState(!checkBoxPCX4Button->getCheckedState());
        });
        ui.views.push_back(checkBoxPCX4Button);

        checkBoxPCX8Button->setSize(150, 50);
        checkBoxPCX8Button->setPosition(300, 550);
        checkBoxPCX8Button->setText("PCX 8bit");
        checkBoxPCX8Button->setTextColor(buttonsTextColor);
        checkBoxPCX8Button->setBackgroundColor(buttonsColor);
        checkBoxPCX8Button->setTextStyle(sf::Text::Style::Bold);
        checkBoxPCX8Button->setTextSize(18);
        checkBoxPCX8Button->setOnClickListener([this](View *v, MouseEvent event) {
            checkBoxPCX8Button->setCheckedState(!checkBoxPCX8Button->getCheckedState());
        });
        ui.views.push_back(checkBoxPCX8Button);

        checkBoxPCX24Button->setSize(150, 50);
        checkBoxPCX24Button->setPosition(450, 550);
        checkBoxPCX24Button->setText("PCX 24bit");
        checkBoxPCX24Button->setTextColor(buttonsTextColor);
        checkBoxPCX24Button->setBackgroundColor(buttonsColor);
        checkBoxPCX24Button->setTextStyle(sf::Text::Style::Bold);
        checkBoxPCX24Button->setTextSize(18);
        checkBoxPCX24Button->setOnClickListener([this](View *v, MouseEvent event) {
            checkBoxPCX24Button->setCheckedState(!checkBoxPCX24Button->getCheckedState());
        });
        ui.views.push_back(checkBoxPCX24Button);

        buttonLoadLogo->setSize(100, 50);
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

        buttonPrintLogo->setSize(100, 50);
        buttonPrintLogo->setPosition(700, 50);
        buttonPrintLogo->setText("Print Logo");
        buttonPrintLogo->setTextColor(buttonsTextColor);
        buttonPrintLogo->setBackgroundColor(buttonsColor);
        buttonPrintLogo->setTextStyle(sf::Text::Style::Bold);
        buttonPrintLogo->setTextSize(18);
        buttonPrintLogo->setOnClickListener([this](View *v, MouseEvent event) {
            printLogo();
        });
        ui.views.push_back(buttonPrintLogo);

        buttonHideLogo->setSize(100, 50);
        buttonHideLogo->setPosition(700, 100);
        buttonHideLogo->setText("Hide Logo");
        buttonHideLogo->setTextColor(buttonsTextColor);
        buttonHideLogo->setBackgroundColor(buttonsColor);
        buttonHideLogo->setTextStyle(sf::Text::Style::Bold);
        buttonHideLogo->setTextSize(18);
        buttonHideLogo->setOnClickListener([this](View *v, MouseEvent event) {
            hideLogo();
        });
        ui.views.push_back(buttonHideLogo);

        encodeDataButton->setSize(100, 50);
        encodeDataButton->setPosition(700, 150);
        encodeDataButton->setText("Encode");
        encodeDataButton->setTextColor(buttonsTextColor);
        encodeDataButton->setBackgroundColor(buttonsColor);
        encodeDataButton->setTextStyle(sf::Text::Style::Bold);
        encodeDataButton->setTextSize(18);
        encodeDataButton->setOnClickListener([this](View *v, MouseEvent event) {
            encodeToImage("data.txt");
        });
        ui.views.push_back(encodeDataButton);

        decodeDataButton->setSize(100, 50);
        decodeDataButton->setPosition(700, 200);
        decodeDataButton->setText("Decode");
        decodeDataButton->setTextColor(buttonsTextColor);
        decodeDataButton->setBackgroundColor(buttonsColor);
        decodeDataButton->setTextStyle(sf::Text::Style::Bold);
        decodeDataButton->setTextSize(18);
        decodeDataButton->setOnClickListener([this](View *v, MouseEvent event) {
            decodeToFile("decoded.txt");
        });
        ui.views.push_back(decodeDataButton);

        uiVisibilityButton->setSize(100, 50);
        uiVisibilityButton->setPosition(700, 550);
        uiVisibilityButton->setText("UI");
        uiVisibilityButton->setTextColor(buttonsTextColor);
        uiVisibilityButton->setBackgroundColor(buttonsColor);
        uiVisibilityButton->setTextStyle(sf::Text::Style::Bold);
        uiVisibilityButton->setTextSize(18);
        uiVisibilityButton->setOnClickListener([this](View *v, MouseEvent event) {
            uiVisible = !uiVisible;
            showUI(uiVisible);
        });
        ui.views.push_back(uiVisibilityButton);
    }

    void loadImage(string fileName) {
        string filePath = currentDir + imageDir + fileName;
        ImageBMP image;
        image.readFromFile(filePath);
        currentImageBitmap = image.toBitmap();
        imageView->setImage(currentImageBitmap);
    }

    void loadPCXImage(string fileName) {
        string filePath = currentDir + imageDir + fileName;
        ImagePCX image;
        image.readFromFile(filePath);
        currentImageBitmap = image.toBitmap();
        imageView->setImage(currentImageBitmap);
    }

    void loadLogo(string fileName) {
        string filePath = currentDir + imageDir + fileName;
        ImageBMP image;
        image.readFromFile(filePath);
        currentLogoBitmap = image.toBitmap();
        logoImageView->setImage(currentLogoBitmap);
        logoImageView->setPosition(width / 2 - currentLogoBitmap->width / 2,
                                   height / 2 - currentLogoBitmap->height / 2);
        logoImageView->setSize(currentLogoBitmap->width, currentLogoBitmap->height);
        logoImageView->setImageScale(imageView->getImageScale());
        logoImageView->setVisible(true);
    }

    void printLogo() {
        if (currentImageBitmap != nullptr && currentLogoBitmap != nullptr) {
            auto logoPositionX =
                    logoImageView->getPosition().x + logoImageView->getImagePosition().x - imageView->getPosition().x -
                    imageView->getImagePosition().x;
            logoPositionX /= imageView->getImageScale().x;
            auto logoPositionY =
                    logoImageView->getPosition().y + logoImageView->getImagePosition().y - imageView->getPosition().y -
                    imageView->getImagePosition().y;
            logoPositionY /= imageView->getImageScale().y;
            LogoEffect logoEffect = LogoEffect(currentLogoBitmap, logoPositionX, logoPositionY, 1.0f);
            logoEffect.applyTo(currentImageBitmap);
            imageView->setImage(currentImageBitmap);
        }
    }

    void hideLogo() {
        if (currentImageBitmap != nullptr && currentLogoBitmap != nullptr) {
            currentLogoBitmap->clear();
            currentLogoBitmap = nullptr;
            logoImageView->setImage(nullptr);
            logoImageView->setVisible(false);
        }
    }

    void encodeToImage(string fileName) {
        CoderData data;
        data.readFromFile(currentDir + imageDir + fileName);
        BitmapCoder::encodeTo(currentImageBitmap, data);
        saveImage("encoded.bmp", BMPType::COLOR24BIT);
    }

    void decodeToFile(string fileName) {
        CoderData data = BitmapCoder::decodeFrom(currentImageBitmap);
        data.writeToFile(currentDir + imageDir + fileName);
    }

    void saveImage(string fileName, BMPType type) {
        string filePath = currentDir + imageDir + fileName;
        ImageBMP image;
        image.fromBitmap(currentImageBitmap);
        image.writeToFile(filePath, type);
    }

    void showUI(bool uiEnabled) {
        blackWhiteButton->setVisible(uiEnabled);
        buttonLoadLogo->setVisible(uiEnabled);
        buttonPrintLogo->setVisible(uiEnabled);
        buttonHideLogo->setVisible(uiEnabled);
        flipVerticalButton->setVisible(uiEnabled);
        flipHorizontalButton->setVisible(uiEnabled);
        rotateRightButton->setVisible(uiEnabled);
        rotateLeftButton->setVisible(uiEnabled);
        addFrameButton->setVisible(uiEnabled);
        cutFrameButton->setVisible(uiEnabled);
        scaleInButton->setVisible(uiEnabled);
        scaleOutButton->setVisible(uiEnabled);
        loadImageButton->setVisible(uiEnabled);
        saveImageButton->setVisible(uiEnabled);
        encodeDataButton->setVisible(uiEnabled);
        decodeDataButton->setVisible(uiEnabled);

        checkBoxBMP4Button->setVisible(uiEnabled);
        checkBoxBMP8Button->setVisible(uiEnabled);
        checkBoxBMP24Button->setVisible(uiEnabled);
        checkBoxPCX4Button->setVisible(uiEnabled);
        checkBoxPCX8Button->setVisible(uiEnabled);
        checkBoxPCX24Button->setVisible(uiEnabled);
    }

    void onDestroy() override {

    }

    void onStart() override {

    }
};


#endif //IMAGEANALYSER_MAINWINDOW_H
