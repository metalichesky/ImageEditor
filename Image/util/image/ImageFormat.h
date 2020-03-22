//
// Created by dmitriy on 22.03.2020.
//

#ifndef IMAGEANALYSER_IMAGEFORMAT_H
#define IMAGEANALYSER_IMAGEFORMAT_H

#include <iostream>
#include <cstdlib>
#include <map>

using namespace std;

class ImageFile;

enum class FormatType {
    BMP, PCX, NONE
};

enum class ColorDepth : uint {
    COLOR1BIT = 1,
    COLOR2BIT = 2,
    COLOR4BIT = 4,
    COLOR8BIT = 8,
    COLOR16BIT = 16,
    COLOR24BIT = 24,
    COLOR32BIT = 32,
    NONE
};

class ImageFormat {
public:
    string formatSuffix = "";
    FormatType type = FormatType::NONE;
    ColorDepth colorDepth = ColorDepth::NONE;

    static ImageFormat getFormatByFilename(string fileName);
protected:
    static map<string, FormatType> createFormatMapping();
    static map<string, FormatType> formatTypeMapping;
};


#endif //IMAGEANALYSER_IMAGEFORMAT_H
