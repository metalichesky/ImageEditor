//
// Created by dmitriy on 18.03.2020.
//

#ifndef IMAGEANALYSER_LOGOEFFECT_H
#define IMAGEANALYSER_LOGOEFFECT_H

#include "../Bitmap.h"
#include "../Math.h"

class LogoEffect : public Effect{
public:
    Bitmap *logo;
    int logoPositionX = 0;
    int logoPositionY = 0;
    float logoIntense = 1.0f;

    LogoEffect(Bitmap *logoBitmap, int logoPositionX, int logoPositionY, float logoIntense) {
        this->logo = logoBitmap;
        this->logoPositionX = logoPositionX;
        this->logoPositionY = logoPositionY;
        this->logoIntense = Math::clamp(logoIntense, 0.0f, 1.0f);
    }
    void applyTo(Bitmap *bitmap) override{
        float pixelIntense = 1.0f;
        float logoPixelIntense = 1.0f;
        for (int x = 0; x < logo->width && x < bitmap->width - logoPositionX; x++) {
            for (int y = 0; y < logo->height && y < bitmap->height - logoPositionY; y++) {
                int pixelX = logoPositionX + x;
                int pixelY = logoPositionY + y;
                if (pixelX < 0 || pixelY < 0) continue;
                Color *currentPixelColor = bitmap->getPixel(pixelX, pixelY);
                Color *logoPixel = logo->getPixel(x, y);
                logoPixelIntense = ((float)logoPixel->a / 255.0f) * logoIntense;
                pixelIntense = 1.0f - logoPixelIntense;
                currentPixelColor->r = Math::clamp((int) (currentPixelColor->r * pixelIntense + logoPixel->r * logoPixelIntense ), 0, 255);
                currentPixelColor->g = Math::clamp((int) (currentPixelColor->g * pixelIntense + logoPixel->g * logoPixelIntense ), 0, 255);
                currentPixelColor->b = Math::clamp((int) (currentPixelColor->b * pixelIntense + logoPixel->b * logoPixelIntense ), 0, 255);
                currentPixelColor->a = Math::clamp((int) (currentPixelColor->a * pixelIntense + logoPixel->a * logoPixelIntense ), 0, 255);
            }
        }
    }

};


#endif //IMAGEANALYSER_LOGOEFFECT_H
