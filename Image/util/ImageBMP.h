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

    static const int16_t BF_TYPE;
    int paletteColorsCount = 0;
    Color *pixels = nullptr;
    int width = 0;
    int height = 0;



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
        width = imageHeader.width;
        height = imageHeader.height;

        read24BitsImage(&fileStream);

        fileStream.close();
        printFileHeader();
        printImageHeader();
        printPaletteColors();
    }

    void writeToFile(const string &filePath) {
        cout << "Writing BMP image file " + filePath << endl;
        fstream fileStream;
        fileStream.open(filePath, fstream::out | fstream::binary);

        write24BitsImage(&fileStream);

        printFileHeader();
        printImageHeader();
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

    void printImageHeader() {
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
                int idx = y * imageHeader.width + x;
                bitmap->setPixel(x, y, pixels[idx]);
            }
        }
        return bitmap;
    }

    void fromBitmap(Bitmap *bitmap) {
        width = bitmap->width;
        height = bitmap->height;
        int pixelsCount = width * height;
        pixels = new Color[pixelsCount];
        for (int i = 0; i < pixelsCount; i++) {
            pixels[i] = bitmap->getPixel(i)->copy();
        }
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
        width = 0;
        height = 0;
        paletteColorsCount = 0;
    }


private:

    static char *toBytes(void *someValue) {
        char *bytes = new char[2];
        std::copy(static_cast<const char *>(static_cast<const void *>(&someValue)),
                  static_cast<const char *>(static_cast<const void *>(&someValue)) + sizeof(someValue),
                  bytes);
        return bytes;
    }

    void read24BitsImage(fstream *fileStream) {
        pixels = new Color[width*height];
        int additionalBytesToRead = imageHeader.width % 4;
        int8_t byte;
        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);

        int idx = 0;
        BMP24Color color;
        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) {
                idx = i * width + j;
                fileStream->read((char *) &color, sizeof(BMP24Color));
                pixels[idx].r = color.red;
                pixels[idx].g = color.green;
                pixels[idx].b = color.blue;
            }
            for (int k = 0; k < additionalBytesToRead; k++) {
                fileStream->read((char *) &byte, sizeof(byte));
            }
        }
    }

    void write24BitsImage(fstream *fileStream) {
        int additionalBytesToWrite = width % 4;
        int pixelsCount = width*height;
        fileHeader.bfType = BF_TYPE;
        fileHeader.bfSize = sizeof(fileHeader)
                + sizeof(imageHeader)
                + sizeof(BMPPaletteItem) * paletteColorsCount
                + sizeof(BMP24Color) * pixelsCount
                + additionalBytesToWrite * height;
        fileHeader.bfOffsetBits = sizeof(fileHeader)
                + sizeof(imageHeader)
                + sizeof(BMPPaletteItem) * paletteColorsCount;
        fileHeader.bfReserved1 = 0;
        fileHeader.bfReserved2 = 0;
        imageHeader.size = sizeof(BMPImageHeader);
        imageHeader.width = width;
        imageHeader.height = height;
        imageHeader.planesCount = 1;
        imageHeader.bitsPerPixel = 24;
        imageHeader.compressionType = 0;
        imageHeader.imageSize = 0;
        imageHeader.xPixelsPerMeter = 2048;
        imageHeader.yPixelsPerMeter = 2048;
        imageHeader.colorsCount = paletteColorsCount;
        imageHeader.colorsImportantCount = paletteColorsCount;

        fileStream->write((char *) &fileHeader, sizeof(BMPFileHeader));
        fileStream->write((char *) &imageHeader, sizeof(BMPImageHeader));

        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);

        int idx = 0;
        int8_t byte = 0;
        BMP24Color color;
        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) {
                idx = i * width + j;
                color.red = pixels[idx].r;
                color.green = pixels[idx].g;
                color.blue = pixels[idx].b;
                fileStream->write((char *) &color, sizeof(BMP24Color));
            }
            for (int k = 0; k < additionalBytesToWrite; k++) {
                fileStream->write((char *) &byte, sizeof(byte));
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
