//
// Created by dmitriy on 09.03.2020.
//

#ifndef IMAGEANALYSER_IMAGEVIEW_H
#define IMAGEANALYSER_IMAGEVIEW_H


#include "View.h"
#include "../util/Bitmap.h"

class ImageView: public View {
public:

    void draw(sf::RenderWindow &window) override {
        View::draw(window);
        prepareSprite();
        window.draw(imageSprite);
    }

    void setImage(Bitmap *newBitmap) {
        setImage(newBitmap->toSFMLImage());
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
    }

    void setImageScale(sf::Vector2f newScale) {
        imageScale = newScale;
        calcImagePosition();
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

protected:
    sf::Sprite imageSprite;
    sf::Texture imageTexture;
    sf::Image image;

    sf::Vector2f imageScale = sf::Vector2f(1.0f, 1.0f);
    sf::Vector2f imagePosition = sf::Vector2f(0.0f, 0.0f);

    float imageRotation = 0;

    void prepareSprite() {
        calcImageScale();
//        imageSprite.setColor(sf::Color::Black);
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
        float diffX = imageWidth - size.x;
        float diffY = imageHeight - size.y;

        if (diffX > diffY) {
            scale = size.x / imageWidth;
        } else {
            scale = size.y / imageHeight;
        }
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
