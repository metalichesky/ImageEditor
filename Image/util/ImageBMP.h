//
// Created by bulgakov on 02.03.2020.
//

#ifndef MANDELBROTFRACTAL_IMAGEBMP_H
#define MANDELBROTFRACTAL_IMAGEBMP_H

#include <iostream>
#include <fstream>
#include <cstdint>
#include <cmath>
#include <sstream>
#include <bitset>
#include "Bitmap.h"
#include "palette/Palette.h"

using namespace std;

enum class BMPType {
    COLOR1BIT, COLOR4BIT, COLOR8BIT, COLOR16BIT, COLOR24BIT, COLOR32BIT
};

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
        paletteColorsCount = imageHeader.bitsPerPixel > 8 ? imageHeader.colorsCount :
                             imageHeader.colorsCount > 0 ? imageHeader.colorsCount : pow(2, imageHeader.bitsPerPixel);
        colors = new BMPPaletteItem[paletteColorsCount];
        if (paletteColorsCount > 0) {
            for (int i = 0; i < paletteColorsCount; i++) {
                fileStream.read((char *) &(colors[i]), sizeof(BMPPaletteItem));
            }
        }
        width = imageHeader.width;
        height = imageHeader.height;

        switch (imageHeader.bitsPerPixel) {
            case 32 :
                readColor32Image(&fileStream);
                break;
            case 24 :
                readColor24Image(&fileStream);
                break;
            case 16 :
                readColor16Image(&fileStream);
                break;
            case 8 :
                readColor8Image(&fileStream);
                break;
            case 4 :
                readColor4Image(&fileStream);
                break;
            case 1 :
                readColor1Image(&fileStream);
        }

        fileStream.close();
        printFileHeader();
        printImageHeader();
        printPaletteColors();
    }

    void writeToFile(const string &filePath, BMPType type = BMPType::COLOR24BIT) {
        cout << "Writing BMP image file " + filePath << endl;
        fstream fileStream;
        fileStream.open(filePath, fstream::out | fstream::binary);

        switch (type) {
            case BMPType::COLOR32BIT :
                writeColor32Image(&fileStream);
                break;
            case BMPType::COLOR24BIT :
                writeColor24Image(&fileStream);
                break;
            case BMPType::COLOR8BIT :
                writeColor8Image(&fileStream);
                break;
            case BMPType::COLOR4BIT :
                writeColor4Image(&fileStream);
                break;
            case BMPType::COLOR1BIT :
                writeColor4Image(&fileStream);
                break;
        }

        printFileHeader();
        printImageHeader();
        fileStream.close();
    }

    void printFileHeader() {
        cout << "--- BMP file header ---" << endl;
        char *bytes = toBytes((void *) fileHeader.bfType);
        cout << "File type: " << bytes << endl;
        cout << "File bounds: " << fileHeader.bfSize << " Bytes or " << fileHeader.bfSize / 20 << endl;
        cout << "Reserved1: " << (int) fileHeader.bfReserved1 << endl;
        cout << "Reserved2: " << (int) fileHeader.bfReserved2 << endl;
        cout << "Offset: " << (int) fileHeader.bfOffsetBits << endl;
    }

    void printImageHeader() {
        cout << "--- BMP map info ---" << endl;
        cout << "InfoHeader bounds: " << imageHeader.size << endl;
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
        cout << "Palette colors " << "(bounds = " << paletteColorsCount << " )" << endl;
        for (int i = 0; i < paletteColorsCount; i++) {
            cout << "r = " << (int) colors[i].red << ", g = " << (int) colors[i].green << ", b = " << (int) colors->blue
                 << endl;
        }
    }

    Bitmap *toBitmap() {
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

    ~ImageBMP() {
        clear();
    }

    void clear() {
        if (pixels != nullptr) {
            delete[] pixels;
        }
        if (colors != nullptr) {
            delete[] colors;
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

    void readColor32Image(fstream *fileStream) {
        pixels = new Color[width * height];
        int additionalBytesToRead = calcPadding(32);
        uint8_t byte;
        uint pixelIdx = 0;
        BMP32Color color;

        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);
        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) {
                pixelIdx = i * width + j;
                fileStream->read((char *) &color, sizeof(BMP32Color));
                pixels[pixelIdx].r = color.red;
                pixels[pixelIdx].g = color.green;
                pixels[pixelIdx].b = color.blue;
                pixels[pixelIdx].a = color.alpha;
            }
            for (int k = 0; k < additionalBytesToRead; k++) {
                fileStream->read((char *) &byte, sizeof(byte));
            }
        }
    }

    void readColor24Image(fstream *fileStream) {
        pixels = new Color[width * height];
        int additionalBytesToRead = calcPadding(24);
        uint8_t byte;
        uint pixelIdx = 0;
        BMP24Color color;

        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);
        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) {
                pixelIdx = i * width + j;
                fileStream->read((char *) &color, sizeof(BMP24Color));
                pixels[pixelIdx].r = color.red;
                pixels[pixelIdx].g = color.green;
                pixels[pixelIdx].b = color.blue;
            }
            for (int k = 0; k < additionalBytesToRead; k++) {
                fileStream->read((char *) &byte, sizeof(byte));
            }
        }
    }

    void readColor16Image(fstream *fileStream) {
        pixels = new Color[width * height];
        int additionalBytesToRead = calcPadding(16);
        uint8_t byte;
        uint pixelIdx = 0;
        BMP16Color1 color{};

        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);
        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) {
                pixelIdx = i * width + j;
                fileStream->read((char *) &color, sizeof(BMP16Color1));
                pixels[pixelIdx].r = color.red;
                pixels[pixelIdx].g = color.green;
                pixels[pixelIdx].b = color.blue;
            }
            for (int k = 0; k < additionalBytesToRead; k++) {
                fileStream->read((char *) &byte, sizeof(byte));
            }
        }
    }

    void readColor8Image(fstream *fileStream) {
        pixels = new Color[width * height];
        int additionalBytesToRead = calcPadding(8);
        uint8_t byte;
        uint pixelIdx = 0;
        BMPPaletteItem paletteColor{};
        uint8_t colorIndex = 0;

        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);
        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) {
                pixelIdx = i * width + j;
                fileStream->read((char *) &colorIndex, sizeof(colorIndex));
                if (colorIndex < paletteColorsCount) {
                    paletteColor = colors[colorIndex];
                    pixels[pixelIdx].r = paletteColor.red;
                    pixels[pixelIdx].g = paletteColor.green;
                    pixels[pixelIdx].b = paletteColor.blue;
                }
            }
            for (int k = 0; k < additionalBytesToRead; k++) {
                fileStream->read((char *) &byte, sizeof(byte));
            }
        }
    }

    void readColor4Image(fstream *fileStream) {
        pixels = new Color[width * height];
        int additionalBytesToRead = calcPadding(4);
        uint8_t byte = 0;
        uint pixelIdx = 0;
        BMPPaletteItem paletteColor{};
        uint8_t colorIndex = 0;
        uint8_t bitsPerPixel = 4;
        uint8_t mask = 0B1111;

        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);
        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width;) {
                fileStream->read((char *) &byte, sizeof(byte));
                for (uint k = 0; k < 2; k++) {
                    colorIndex = mask & (byte >> (k * bitsPerPixel));
                    pixelIdx = i * width + j;
                    if (colorIndex < paletteColorsCount) {
                        paletteColor = colors[colorIndex];
                        pixels[pixelIdx].r = paletteColor.red;
                        pixels[pixelIdx].g = paletteColor.green;
                        pixels[pixelIdx].b = paletteColor.blue;
                    }
                    j++;
                }
            }
            for (int k = 0; k < additionalBytesToRead; k++) {
                fileStream->read((char *) &byte, sizeof(byte));
            }
        }
    }

    void readColor1Image(fstream *fileStream) {
        pixels = new Color[width * height];
        int additionalBytesToRead = calcPadding(1);
        int8_t byte;
        uint pixelIdx = 0;
        BMPPaletteItem paletteColor{};
        uint8_t colorIndex = 0;
        uint8_t bitsPerPixel = 1;
        uint8_t mask = 0B1;

        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);
        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width;) {
                fileStream->read((char *) &colorIndex, sizeof(colorIndex));
                for (uint k = 0; k < 8; k++) {
                    colorIndex = mask & (colorIndex >> (k * bitsPerPixel));
                    pixelIdx = i * width + j;
                    if (colorIndex < paletteColorsCount) {
                        paletteColor = colors[colorIndex];
                        pixels[pixelIdx].r = paletteColor.red;
                        pixels[pixelIdx].g = paletteColor.green;
                        pixels[pixelIdx].b = paletteColor.blue;
                    }
                    j++;
                }
            }
            for (int k = 0; k < additionalBytesToRead; k++) {
                fileStream->read((char *) &byte, sizeof(byte));
            }
        }
    }

    void writeColor32Image(fstream *fileStream) {
        int additionalBytesPerRow = calcPadding(32);
        writeHeaders(fileStream, 32, additionalBytesPerRow);
        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);

        int idx = 0;
        int8_t byte = 0;
        BMP32Color color;
        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) {
                idx = i * width + j;
                color.red = pixels[idx].r;
                color.green = pixels[idx].g;
                color.blue = pixels[idx].b;
                color.alpha = pixels[idx].a;
                fileStream->write((char *) &color, sizeof(BMP32Color));
            }
            for (int k = 0; k < additionalBytesPerRow; k++) {
                fileStream->write((char *) &byte, sizeof(byte));
            }
        }
    }

    void writeColor24Image(fstream *fileStream) {
        int additionalBytesPerRow = calcPadding(24);
        writeHeaders(fileStream, 24, additionalBytesPerRow);
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
            for (int k = 0; k < additionalBytesPerRow; k++) {
                fileStream->write((char *) &byte, sizeof(byte));
            }
        }
    }

    void writeColor8Image(fstream *fileStream) {
        int additionalBytesPerRow = calcPadding(8);
        paletteColorsCount = 256;
        writeHeaders(fileStream, 8, additionalBytesPerRow);

        Palette palette(pixels, width * height);
        cout << "Change colors count form " << palette.colors->size() << " to " << paletteColorsCount << "..." << endl;
        MedianSection median;
        palette.resize(paletteColorsCount, median);
        writePalette(fileStream, palette);

        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);
        int idx = 0;
        int8_t byte = 0;
        uint8_t paletteIndex = 0;
        Color currentPixel;
        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width; j++) {
                idx = i * width + j;
                currentPixel = pixels[idx];
                paletteIndex = palette.getNearestIndex(currentPixel);
                fileStream->write((char *) &paletteIndex, sizeof(paletteIndex));
            }
            for (int k = 0; k < additionalBytesPerRow; k++) {
                fileStream->write((char *) &byte, sizeof(byte));
            }
        }
    }

    void writeColor4Image(fstream *fileStream) {
        int additionalBytesPerRow = calcPadding(4);
        paletteColorsCount = 16;
        writeHeaders(fileStream, 4, additionalBytesPerRow);

        Palette palette(pixels, width * height);
        cout << "Change colors count form " << palette.colors->size() << " to " << paletteColorsCount << "..." << endl;
        MedianSection median;
        palette.resize(paletteColorsCount, median);

        writePalette(fileStream, palette);
        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);

        int idx = 0;
        uint8_t byte = 0;
        uint8_t paletteIndex = 0;
        Color currentPixel;
        uint8_t mask = 0B1111;
        uint8_t bitsPerPixel = 4;
        for (int i = height - 1; i >= 0; i--) {
            for (int j = 0; j < width;) {
                byte = 0;
                for (int k = 0; k < 2 && j < width; k++) {
                    idx = i * width + j;
                    currentPixel = pixels[idx];
                    paletteIndex = palette.getNearestIndex(currentPixel);
                    byte = byte | ((uint)(mask & paletteIndex) << (uint)((2 - k - 1) * bitsPerPixel));
                    j++;
                }
                fileStream->write((char *) &byte, sizeof(byte));
            }
            byte = 0;
            for (int k = 0; k < additionalBytesPerRow; k++) {
                fileStream->write((char *) &byte, sizeof(byte));
            }
        }
    }

    void writeHeaders(fstream *fileStream, int bitsPerPixel, int additionalBytesPerRow) {
        int pixelsCount = width * height;
        fileHeader.bfType = BF_TYPE;
        fileHeader.bfSize = sizeof(fileHeader)
                            + sizeof(imageHeader)
                            + sizeof(BMPPaletteItem) * paletteColorsCount
                            + pixelsCount * bitsPerPixel / 8
                            + additionalBytesPerRow * height;
        fileHeader.bfOffsetBits = sizeof(fileHeader)
                                  + sizeof(imageHeader)
                                  + sizeof(BMPPaletteItem) * paletteColorsCount;
        fileHeader.bfReserved1 = 0;
        fileHeader.bfReserved2 = 0;
        imageHeader.size = sizeof(BMPImageHeader);
        imageHeader.width = width;
        imageHeader.height = height;
        imageHeader.planesCount = 1;
        imageHeader.bitsPerPixel = bitsPerPixel;
        imageHeader.compressionType = 0;
        imageHeader.imageSize = (bitsPerPixel * pixelsCount) / 8 + additionalBytesPerRow * height;
        imageHeader.xPixelsPerMeter = 0;
        imageHeader.yPixelsPerMeter = 0;
        imageHeader.colorsCount = paletteColorsCount;
        imageHeader.colorsImportantCount = paletteColorsCount;
        fileStream->write((char *) &fileHeader, sizeof(BMPFileHeader));
        fileStream->write((char *) &imageHeader, sizeof(BMPImageHeader));
    }

    void writePalette(fstream *fileStream, Palette &palette) {
        BMPPaletteItem paletteItem{};
        for (auto color: *palette.colors) {
            paletteItem.red = color.r;
            paletteItem.green = color.g;
            paletteItem.blue = color.b;
            paletteItem.reversed = color.a;
            fileStream->write((char *) &paletteItem, sizeof(BMPPaletteItem));
        }
    }

    int calcPadding(int bitsPerPixel) {
        int padding = 4 - ((int)ceil((((double) bitsPerPixel * width) / 8.0))) % 4;
        padding =  padding == 4? 0 : padding;
        cout << "Padding: " << padding << endl;
        return padding;
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
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t reversed;
    } *colors = nullptr;
#pragma pack(pop)

#pragma pack(push, 1)
    typedef struct BMP24Color {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    } BMP24Color;
#pragma pack(pop)

#pragma pack(push, 1)
    typedef struct BMP32Color {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    } BMP32Color;
#pragma pack(pop)

#pragma pack(push, 1)
    typedef struct BMP16Color1 {
        uint8_t blue : 5;
        uint8_t green : 6;
        uint8_t red : 5;
    } BMP8Color1;
#pragma pack(pop)

#pragma pack(push, 1)
    typedef struct BMP16Color2 {
        uint8_t blue : 5;
        uint8_t green : 5;
        uint8_t red : 5;
        uint8_t additional : 1;
    } BMP8Color2;
#pragma pack(pop)

};

#endif //MANDELBROTFRACTAL_IMAGEBMP_H
