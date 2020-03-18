//
// Created by dmitriy on 14.03.2020.
//

#ifndef IMAGEANALYSER_PALETTE_H
#define IMAGEANALYSER_PALETTE_H


#include "../Color.h"
#include "ColorMetadata.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include "MedianSection.h"
#include "Quantization.h"
#include "../sorting/Comparator.h"
#include "../sorting/QuickSorter.h"

using namespace std;

class Palette {
public:
    vector<Color> *colors = new vector<Color>;
    vector<ColorMetadata> *colorsData = new vector<ColorMetadata>;

    Palette(Color *newColors, int newColorsCount) {
        cout << "Preparing palette for " << newColorsCount << " colors..."<< endl;
        vector<Color*> tempVector(newColorsCount);
        Color* colorPointer = nullptr;
        for (int i = 0; i < newColorsCount; i++) {
            tempVector[i] = &newColors[i];
        }
        sort(tempVector.begin(), tempVector.end(), SortColorPointers());
        Color* currentColor = nullptr;
        Color* previousColor = nullptr;
        int colorsCounter = -1;
        for (int i = 0; i < newColorsCount; i++) {
            currentColor = tempVector.at(i);
            if (currentColor != previousColor) {
                colors->push_back(*currentColor);
                colorsData->push_back(ColorMetadata(currentColor, 1));
                colorsCounter++;
            } else {
                colorsData->at(colorsCounter).count++;
            }
            previousColor = currentColor;
        }

        cout << "Palette ready" << endl;
    }

    int getNearestIndex(Color &color) {
        int idx = 0;
        double minDistance = MAXFLOAT;
        double currentDistance = 0.0;
        for (int i = 0; i < colors->size(); i++) {
            currentDistance = color.distanceTo(colors->at(i));
            if (currentDistance < minDistance) {
                minDistance = currentDistance;
                idx = i;
            }
        }
        return idx;
    }

    Color getNearestColor(Color &color) {
        return colors->at(getNearestIndex(color));
    }

    void resize(int newColorsCount, Quantization &quantization) {
        auto newColors = quantization.quantizeColors(colorsData, newColorsCount);
        delete colors;
        colors = newColors;
    }

//    void changeColorsCount(int newColorsCount) {
//        if (newColorsCount > colors->size()) {
//
//        } else if (newColorsCount < colors->size()) {
//            auto *newColors = new vector<Color>();
//            double oldPerNew = (double) colors->size() / (double) newColorsCount;
//            int colorsCounter = 0;
//            int colorAccumulator[4];
//            for (int i = 0; i < newColorsCount; i++) {
//                for (int &j : colorAccumulator) {
//                    j = 0;
//                }
//                colorsCounter = 0;
//                for (int j = (int) round(i * oldPerNew); j < ((i + 1) * oldPerNew) && j < colors->size(); j++) {
//                    colorAccumulator[0] += colors->at(j).r;
//                    colorAccumulator[1] += colors->at(j).g;
//                    colorAccumulator[2] += colors->at(j).b;
//                    colorAccumulator[3] += colors->at(j).a;
//                    colorsCounter++;
//                }
//                Color newColor;
//                if (colorsCounter > 0) {
//                    newColor.r = min(colorAccumulator[0] / colorsCounter, 255);
//                    newColor.g = min(colorAccumulator[1] / colorsCounter, 255);
//                    newColor.b = min(colorAccumulator[2] / colorsCounter, 255);
//                    newColor.a = min(colorAccumulator[3] / colorsCounter, 255);
//                }
//                newColors->push_back(newColor);
//            }
//            delete colors;
//            colors = newColors;
//        }
//    }

    pair<bool, int > findColor(const Color &element)
    {
        pair<bool, int> result;
        // Find given element in vector
        auto it = find(colors->begin(), colors->end(), element);

        if (it != colors->end()) {
            result.first = true;
            result.second = distance(colors->begin(), it);
        } else {
            result.first = false;
            result.second = -1;
        }
        return result;
    }

    bool colorExists(const Color &color) {
        auto it = find(colors->begin(), colors->end(), color);
        return it != colors->end();
    }

    void print() {
        cout << "Palette colors " << "(bounds = " << colors->size() << " ):" << endl;
        for (auto &color : *colors) {
            color.print();
        }
    }

private:
    struct SortColorPointers
    {
        bool operator()(const Color* one, const Color* two) {
            if (one != nullptr && two != nullptr) return one->b < two->b;
            else return one == nullptr;
        }
    };
};


#endif //IMAGEANALYSER_PALETTE_H
