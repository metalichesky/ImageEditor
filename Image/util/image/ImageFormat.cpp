//
// Created by dmitriy on 22.03.2020.
//
#include "ImageFormat.h"

ImageFormat ImageFormat::getFormatByFilename(string fileName)  {
    ImageFormat format;
    int idxOfDot = fileName.find_last_of('.');
    if (idxOfDot < 0) return format;
    format.formatSuffix = fileName.substr(idxOfDot+1, fileName.size()-idxOfDot);
    auto mappedFormat = formatTypeMapping[format.formatSuffix];
    format.type = mappedFormat;
    return format;
}

map<string, FormatType> ImageFormat::createFormatMapping() {
    map<string, FormatType> map;
    map["pcx"] = FormatType::PCX;
    map["PCX"] = FormatType::PCX;
    map["bmp"] = FormatType::BMP;
    map["BMP"] = FormatType::BMP;
    return map;
}

map<string, FormatType> ImageFormat::formatTypeMapping = createFormatMapping();

