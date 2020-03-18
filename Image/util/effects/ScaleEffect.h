//
// Created by dmitriy on 16.03.2020.
//

#ifndef IMAGEANALYSER_SCALEEFFECT_H
#define IMAGEANALYSER_SCALEEFFECT_H


#include "../Bitmap.h"
#include "../Math.h"
#include <cmath>

using namespace std;

class ScaleEffect : public Effect {
public:

    ScaleEffect() = default;

    ScaleEffect(double newScaleFactor) {
        scaleFactor = newScaleFactor;
    }

    double scaleFactor = 1.0;

    void applyTo(Bitmap *bitmap) override {
        cout << "Scale bitmap by " << scaleFactor << endl;
        int oldWidth = bitmap->width;
        int oldHeight = bitmap->height;
        int newWidth = bitmap->width * scaleFactor;
        int newHeight = bitmap->height * scaleFactor;
        int oldPerNew = ceil(scaleFactor);

        Color *newPixels = new Color[newWidth * newHeight];
        for (int h = 0; h < newHeight; h++) {
            for (int w = 0; w < newWidth; w++) {
                int oldW = round((double) w / scaleFactor);
                oldW = Math::clamp(oldW, 0, oldWidth);
                int oldH = round((double) h / scaleFactor);
                oldH = Math::clamp(oldH, 0, oldHeight);
                Color newColor = *bitmap->getPixel(oldW, oldH);
                if (scaleFactor < 1) {
                    int sumR = 0;
                    int sumG = 0;
                    int sumB = 0;
                    int pixelsCounter = 0;
                    for (int i = oldW; (i < oldW + oldPerNew) && i < oldWidth; i++) {
                        for (int j = oldH; (j < oldH + oldPerNew) && j < oldHeight; j++) {
                            auto oldPixel = bitmap->getPixel(i, j);
                            sumR += oldPixel->r;
                            sumG += oldPixel->g;
                            sumB += oldPixel->b;
                            pixelsCounter++;
                        }
                    }
                    newColor.r = sumR / pixelsCounter;
                    newColor.g = sumG / pixelsCounter;
                    newColor.b = sumB / pixelsCounter;
                }
                int idx = newWidth * h + w;
                newPixels[idx] = newColor;
            }
        }
        bitmap->width = newWidth;
        bitmap->height = newHeight;
        bitmap->setPixels(newPixels);
    }

};


#endif //IMAGEANALYSER_SCALEEFFECT_H
