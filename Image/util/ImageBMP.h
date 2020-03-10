//
// Created by bulgakov on 02.03.2020.
//

#ifndef MANDELBROTFRACTAL_IMAGEBMP_H
#define MANDELBROTFRACTAL_IMAGEBMP_H

#include <iostream>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <bitset>
#include "Bitmap.h"

using namespace std;

class ImageBMP {
public:
    void readFromFile(const string &filePath) {
        cout << "Reading BMP image file " + filePath << endl;
        fstream fileStream;

        fileStream.open(filePath, fstream::in | fstream::binary);

        fileStream.read((char *) &(fileHeader), sizeof(BMPFileHeader));
        fileStream.read((char *) &(imageHeader), sizeof(BMPImageHeader));
        paletteColorsCount = imageHeader.colorsCount;
        if (paletteColorsCount > 0) {
            colors = new BMPPaletteItem[paletteColorsCount];
            for (int i = 0; i < paletteColorsCount; i++) {
                fileStream.read((char *) &(colors[i]), sizeof(BMPImageHeader));
            }
        }
        pixelsCount = imageHeader.width * imageHeader.height;

        read24BitsImage(&fileStream);

        fileStream.close();
        printFileHeader();
        printMapInfo();
        printPaletteColors();
    }

    void writeToFile(const string &filePath) {
        cout << "Writing BMP image file " + filePath << endl;
        fstream fileStream;

        fileStream.open(filePath, fstream::out | fstream::binary);

        fileStream.close();
    }

    void printFileHeader() {
        cout << "--- BMP file header ---" << endl;
        char *bytes = toBytes((void *) fileHeader.bfType);
        cout << "File type: " << bytes << endl;
        cout << "File size: " << fileHeader.bfSize << " Bytes or " << fileHeader.bfSize / 20 << endl;
        cout << "Reserved1: " << (int) fileHeader.bfReserved1 << endl;
        cout << "Reserved2: " << (int) fileHeader.bfReserved2 << endl;
        cout << "Offset: " << (int) fileHeader.bfOffsetBits << endl;
    }

    void printMapInfo() {
        cout << "--- BMP map info ---" << endl;
        cout << "InfoHeader size: " << imageHeader.size << endl;
        cout << "Image width: " << imageHeader.width << endl;
        cout << "Image height: " << imageHeader.height << endl;
        cout << "Planes count: " << imageHeader.planesCount << endl;
        cout << "Bits per pixel: " << imageHeader.bitsPerPixel << endl;
        cout << "Compression type: " << imageHeader.compressionType << endl;
        cout << "Image Size (bytes): " << imageHeader.imageSize << endl;
        cout << "X pixels per meter: " << imageHeader.xPixelsPerMeter << endl;
        cout << "Y pixels per meter: " << imageHeader.yPixelsPerMeter << endl;
        cout << "Colors used count: " << imageHeader.colorsCount << endl;
        cout << "Important colors count: " << imageHeader.colorsImportantCount << endl;
    }

    void printPaletteColors() {
        cout << "Palette colors " << "(size = " << paletteColorsCount << " )" << endl;
        for (int i = 0; i < paletteColorsCount; i++) {
            cout << "r = " << (int) colors[i].red << ", g = " << (int) colors[i].green << ", b = " << (int) colors->blue << endl;
        }
    }

    Bitmap* toBitmap() {
        auto *bitmap = new Bitmap(imageHeader.width, imageHeader.height);
        for (int y = 0; y < imageHeader.height; y++) {
            for (int x = 0; x < imageHeader.width; x++) {
                int idx = (imageHeader.height - y - 1) * imageHeader.width + x;
                bitmap->setPixel(x, y, pixels[idx]);
            }
        }
        return bitmap;
    }

    ~ImageBMP(){
        clear();
    }

    void clear() {
        if (pixels != nullptr) {
            delete(pixels);
        }
        if (colors != nullptr) {
            delete(colors);
        }
        pixels = nullptr;
        colors = nullptr;
        pixelsCount = 0;
        paletteColorsCount = 0;
    }

    int paletteColorsCount = 0;
    Color *pixels = nullptr;
    int pixelsCount = 0;

private:

    static char *toBytes(void *someValue) {
        char *bytes = new char[2];
        std::copy(static_cast<const char *>(static_cast<const void *>(&someValue)),
                  static_cast<const char *>(static_cast<const void *>(&someValue)) + sizeof(someValue),
                  bytes);
        return bytes;
    }

    void read24BitsImage(fstream *fileStream) {
        pixels = new Color[pixelsCount];
        int additionalBytesToRead = imageHeader.width % 4;
        int8_t byte;
        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);

        int pixelsCounter = 0;
        BMP24Color color;
        for (int i = 0; i < imageHeader.height; i++) {
            for (int j = 0; j < imageHeader.width; j++) {
                fileStream->read((char *) &color, sizeof(BMP24Color));
                pixels[pixelsCounter].r = color.red;
                pixels[pixelsCounter].g = color.green;
                pixels[pixelsCounter].b = color.blue;
                pixelsCounter++;
            }
            for (int k = 0; k < additionalBytesToRead; k++) {
                fileStream->read((char *) &byte, sizeof(byte));
            }
        }
    }

#pragma pack(push, 1)
    struct BMPFileHeader {
        int16_t bfType;
        int32_t bfSize;
        int16_t bfReserved1;
        int16_t bfReserved2;
        int32_t bfOffsetBits;
    } fileHeader;
#pragma pack(pop)

#pragma pack(push, 1)
    struct BMPImageHeader {
        int32_t size;
        int32_t width;
        int32_t height;
        int16_t planesCount;
        int16_t bitsPerPixel;
        int32_t compressionType;
        int32_t imageSize;
        int32_t xPixelsPerMeter;
        int32_t yPixelsPerMeter;
        int32_t colorsCount;
        int32_t colorsImportantCount;
    } imageHeader;
#pragma pack(pop)

#pragma pack(push, 1)
    struct BMPPaletteItem {
        int8_t red;
        int8_t green;
        int8_t blue;
        int8_t reversed;
    } *colors = nullptr;
#pragma pack(pop)

#pragma pack(push, 1)
    typedef struct BMP24Color {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    } BMP24Color;
#pragma pack(pop)

};

#endif //MANDELBROTFRACTAL_IMAGEBMP_H
