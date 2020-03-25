//
// Created by dmitriy on 09.03.2020.
//

#ifndef IMAGEANALYSER_IMAGEVIEW_H
#define IMAGEANALYSER_IMAGEVIEW_H


#include "View.h"
#include "../util/Bitmap.h"

class ImageView: public View {
    typedef function<void(View*, sf::Vector2f)> OnScaledListener;
public:

    void draw(sf::RenderWindow &window) override {
        if (isVisible) {
            View::draw(window);
            prepareSprite();
            window.draw(imageSprite);
        }
    }

    void setImage(Bitmap *newBitmap) {
        if (newBitmap != nullptr) {
            setImage(newBitmap->toSFMLImage());
        }
//        else {
//            sf::Image image;
//            image.create(0,0);
//            setImage(image);
//        }
    }

    void setImage(sf::Image newImage) {
        imageTexture.loadFromImage(newImage);
        calcImageScale();
        calcImagePosition();
        imageSprite = sf::Sprite();
    }

    void setImageScale(float scaleX, float scaleY) {
        imageScale.x = scaleX;
        imageScale.y = scaleY;
        calcImagePosition();
        if (onScaledListener != nullptr) {
            (onScaledListener)(this, imageScale);
        }
    }

    void setImageScale(sf::Vector2f newScale) {
        setImageScale(newScale.x, newScale.y);
    }

    sf::Vector2f getImageScale() {
        return imageScale;
    }

    void setImagePosition(float positionX, float positionY) {
        imagePosition.x = positionX;
        imagePosition.y = positionY;
    }

    void setImagePosition(sf::Vector2f newPosition) {
        imagePosition = newPosition;
    }

    sf::Vector2f getImagePosition() {
        return imagePosition;
    }

    void setOnScaledListener(OnScaledListener newOnScaleListener) {
        this->onScaledListener = newOnScaleListener;
    }

protected:
    sf::Sprite imageSprite;
    sf::Texture imageTexture;
    sf::Image image;
    OnScaledListener onScaledListener;
    sf::Vector2f imageScale = sf::Vector2f(1.0f, 1.0f);
    sf::Vector2f imagePosition = sf::Vector2f(0.0f, 0.0f);

    float imageRotation = 0;

    void prepareSprite() {
        imageSprite.setTexture(imageTexture);
        imageSprite.setScale(imageScale);
        imageSprite.setPosition(position.x + imagePosition.x, position.y + imagePosition.y);
        imageSprite.setRotation(imageRotation);
    }

    void calcImageScale() {
        auto imageTextureSize = imageTexture.getSize();
        float imageWidth = imageTextureSize.x;
        float imageHeight = imageTextureSize.y;
        float scale = 0;
        float ratioX = imageWidth / size.x;
        float ratioY = imageHeight / size.y;
        scale = (ratioX > ratioY) ? size.x / imageWidth : size.y / imageHeight;
        imageScale.x = scale;
        imageScale.y = scale;
    }

    void calcImagePosition() {
        auto imageTextureSize = imageTexture.getSize();
        float imageWidth = imageTextureSize.x;
        float imageHeight = imageTextureSize.y;

        float scaledWidth = imageWidth * imageScale.x;
        float scaledHeight = imageHeight * imageScale.y;

        imagePosition.x = (size.x - scaledWidth) / 2;
        imagePosition.y = (size.y - scaledHeight) / 2;
    }

};


#endif //IMAGEANALYSER_IMAGEVIEW_H
