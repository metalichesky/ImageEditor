//
// Created by dmitriy on 16.03.2020.
//

#ifndef IMAGEANALYSER_MEDIANSECTION_H
#define IMAGEANALYSER_MEDIANSECTION_H


#include "Quantization.h"
#include <vector>
#include <algorithm>

class Section{
public:
    vector<ColorMetadata*> colors;
    pair<Color, Color> bounds = pair<Color, Color>(Color::BLACK, Color::BLACK);
    int totalColorsCount = 0;

    void calculateBounds() {
        if (colors.empty()) return;
        auto firstColor = colors.front();
        pair<int, int> rangeRed(firstColor->color->r,firstColor->color->r);
        pair<int, int> rangeGreen(firstColor->color->g,firstColor->color->g);
        pair<int, int> rangeBlue(firstColor->color->b,firstColor->color->b);
        totalColorsCount = 0;
        for (auto &currentColor: colors) {
            totalColorsCount += currentColor->count;
            if (currentColor->color->r < rangeRed.first) {
                rangeRed.first = currentColor->color->r;
            }
            if (currentColor->color->r > rangeRed.second) {
                rangeRed.second = currentColor->color->r;
            }
            if (currentColor->color->g < rangeGreen.first) {
                rangeGreen.first = currentColor->color->g;
            }
            if (currentColor->color->g > rangeGreen.second) {
                rangeGreen.second = currentColor->color->g;
            }
            if (currentColor->color->b < rangeBlue.first) {
                rangeBlue.first = currentColor->color->b;
            }
            if (currentColor->color->b > rangeBlue.second) {
                rangeBlue.second = currentColor->color->b;
            }
        }
        bounds.first.r = rangeRed.first;
        bounds.first.g = rangeGreen.first;
        bounds.first.b = rangeBlue.first;
        bounds.second.r = rangeRed.second;
        bounds.second.g = rangeGreen.second;
        bounds.second.b = rangeBlue.second;
    }

    Color calcAvgColor() {
        Color avgColor = Color::BLACK;
        int sumR = 0;
        int sumG = 0;
        int sumB = 0;
        for (ColorMetadata* colorData: colors) {
            sumR += colorData->color->r * colorData->count;
            sumG += colorData->color->g * colorData->count;
            sumB += colorData->color->b * colorData->count;
        }
        avgColor.r = sumR / totalColorsCount;
        avgColor.g = sumG / totalColorsCount;
        avgColor.b = sumB / totalColorsCount;
        return avgColor;
    }

    Color calcAvgBound() {
        Color avgColor = Color::BLACK;
        avgColor.r = (bounds.first.r + bounds.second.r) / 2;
        avgColor.g = (bounds.first.g + bounds.second.g) / 2;
        avgColor.b = (bounds.first.b + bounds.second.b) / 2;
        return avgColor;
    }
};

class MedianSection : public Quantization {

    vector<Color>* quantizeColors(vector<ColorMetadata> *newColorsData, int newDestinationColorsCount) override {
        this->colorsData = newColorsData;
        this->destinationColorsCount = newDestinationColorsCount;
        cutSections();

        auto *newColors = new vector<Color>();
        for (Section section: *sections) {
            newColors->push_back(section.calcAvgColor());
        }
        return newColors;
    }

private:
    vector<Section> *sections = new vector<Section>();
    vector<ColorMetadata>* colorsData;
    int destinationColorsCount = 0;


    void cutSections() {
        Section cube = Section();
        for (ColorMetadata &color: *colorsData) {
            cube.colors.push_back(&color);
        }
        cube.calculateBounds();
        sections->push_back(cube);

        while(sections->size() < destinationColorsCount) {
            auto *newSections = new vector<Section>();
//            #pragma omp parallel
            for (auto & section : *sections) {
                auto cutted = cutSection(section);
                for (Section &cuttedSection: *cutted) {
                    newSections->push_back(cuttedSection);
                }
            }
            if (sections->size() == newSections->size()) {
                delete newSections;
                break;
            }
            delete sections;
            sections = newSections;
        }
    }

    vector<Section>* cutSection(Section &sectionToCut) {
        auto* newSections = new vector<Section>();

        int rangeRedLength = sectionToCut.bounds.second.r - sectionToCut.bounds.first.r;
        int rangeGreenLength = sectionToCut.bounds.second.g - sectionToCut.bounds.first.g;
        int rangeBlueLength = sectionToCut.bounds.second.b - sectionToCut.bounds.first.b;
        int maxRange = max(rangeRedLength, max(rangeBlueLength, rangeGreenLength));
        uint medianIndex = sectionToCut.colors.size() / 2;

        if (maxRange == rangeRedLength) {
            sort(sectionToCut.colors.begin(), sectionToCut.colors.end(), SortByRed());
        } else if (maxRange == rangeGreenLength) {
            sort(sectionToCut.colors.begin(), sectionToCut.colors.end(), SortByGreen());
        } else {
            sort(sectionToCut.colors.begin(), sectionToCut.colors.end(), SortByBlue());
        }
        ColorMetadata *median = sectionToCut.colors.at(medianIndex);

        auto section1 = Section();
        auto section2 = Section();
        int sectionSize = sectionToCut.colors.size();
        int section1Size = medianIndex;
        int section2Size = sectionSize - medianIndex;
        section1.colors = vector<ColorMetadata*>(section1Size);
        section2.colors = vector<ColorMetadata*>(section2Size);
        for (int i = 0; i < section1Size; i++) {
            section1.colors[i] = sectionToCut.colors[i];
        }
        for (int i = 0; i < section2Size; i++) {
            section2.colors[i] = sectionToCut.colors[medianIndex + i];
        }

        if (section1Size != 0) {
            section1.calculateBounds();
            newSections->push_back(section1);
        }
        if (section2Size != 0) {
            section2.calculateBounds();
            newSections->push_back(section2);
        }
//
//        cout << "Range red " << rangeRed.first << ", " << rangeRed.second << " = " << rangeRedLength << endl;
//        cout << "Range green " << rangeGreen.first << ", " << rangeGreen.second << " = " << rangeGreenLength << endl;
//        cout << "Range blue " << rangeBlue.first << ", " << rangeBlue.second << " = " << rangeBlueLength << endl;
        return newSections;
    }


    struct SortByRed
    {
        inline bool operator()(const ColorMetadata* one, const ColorMetadata* two) {
            return one->color->r < two->color->r;
        }
    };

    struct SortByGreen
    {
        bool operator()(const ColorMetadata* one, const ColorMetadata* two) {
            return one->color->g < two->color->g;
        }
    };

    struct SortByBlue
    {
        bool operator()(const ColorMetadata* one, const ColorMetadata* two) {
            return one->color->b < two->color->b;
        }
    };
};


#endif //IMAGEANALYSER_MEDIANSECTION_H
