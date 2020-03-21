//
// Created by dmitriy on 19.03.2020.
//

#ifndef IMAGEANALYSER_IMAGEPCX_H
#define IMAGEANALYSER_IMAGEPCX_H


#include <iostream>
#include <fstream>
#include <cstdint>
#include <cmath>
#include <sstream>
#include <bitset>
#include "Bitmap.h"
#include "palette/Palette.h"

using namespace std;

enum class PCXType {
    COLOR1BIT, //NumBitPlanes 1, BitsPerPixel 1, Monochrome
    COLOR2BIT, //NumBitPlanes 1, BitsPerPixel 2, CGA
    COLOR3BIT, //NumBitPlanes 3, BitsPerPixel 1, EGA
    COLOR4BIT, //NumBitPlanes 4, BitsPerPixel 1, EGA and VGA
    COLOR8BIT, //NumBitPlanes 1, BitsPerPixel 8, Extended VGA
    COLOR24BIT //NumBitPlanes 3, BitsPerPixel 8, Extended VGA and XGA
};

class ImagePCX {
public:
    static const int8_t PCX_ID;
    static const int8_t PCX_VERSION0;
    static const int8_t PCX_VERSION1;
    static const int8_t PCX_VERSION2;
    static const int8_t PCX_VERSION3;
    static const int8_t PCX_VERSION4;
    static const int8_t PCX_VERSION5;
    static const int8_t PCX_ENCODING_NONE;
    static const int8_t PCX_ENCODING_RLE;
    static const int8_t PCX_RESERVED1;
    static const int8_t PCX_PALETTE_TYPE0;
    static const int8_t PCX_PALETTE_TYPE1;
    static const int8_t PCX_PALETTE_TYPE2;
    int paletteColorsCount = 0;
    Color *pixels = nullptr;
    int width = 0;
    int height = 0;


    void readFromFile(const string &filePath) {
        cout << "Reading PCX image file " + filePath << endl;
        fstream fileStream;

        fileStream.open(filePath, fstream::in | fstream::binary);

        fileStream.read((char *) &(header), sizeof(PCXHeader));

        egaPaletteColors = new PCX24Color[16];
        copy(reinterpret_cast<char *>(&header.palette),
             reinterpret_cast<char *>(&header.palette) + sizeof(PCXHeader::palette),
             reinterpret_cast<char *>(egaPaletteColors));
        width = header.xEnd - header.xStart + 1;
        height = header.yEnd - header.yStart + 1;
        int linePaddingSize = ((header.bytesPerLine * header.numBitPlanes) * (8 / header.bitsPerPixel)) - width;

        int bitPerColor = header.bitsPerPixel * header.numBitPlanes;
        readImage(&fileStream, bitPerColor);

        fileStream.close();

        printEGAPaletteColors();
        printVGAPaletteColors();
        printHeader();
    }
//
//    void writeToFile(const string &filePath, BMPType type = BMPType::COLOR24BIT) {
//        cout << "Writing BMP image file " + filePath << endl;
//        fstream fileStream;
//        fileStream.open(filePath, fstream::out | fstream::binary);
//
//        switch (type) {
//            case BMPType::COLOR32BIT :
//                writeColor32Image(&fileStream);
//                break;
//            case BMPType::COLOR24BIT :
//                writeColor24Image(&fileStream);
//                break;
//            case BMPType::COLOR8BIT :
//                writeColor8Image(&fileStream);
//                break;
//            case BMPType::COLOR4BIT :
//                writeColor4Image(&fileStream);
//                break;
//            case BMPType::COLOR1BIT :
//                writeColor4Image(&fileStream);
//                break;
//        }
//
//        printHeader();
//        printImageHeader();
//        fileStream.close();
//    }

    void printHeader() {
        cout << "--- PCX file header ---" << endl;
        cout << "Identifier: " << hex << (uint) header.identifier << endl;
        cout << "Version: " << dec << (uint) header.version << endl;
        cout << "Encoding: " << (uint) header.encoding << endl;
        cout << "BitsPerPixel: " << (uint) header.bitsPerPixel << endl;
        cout << "XStart: " << header.xStart << endl;
        cout << "YStart: " << header.yStart << endl;
        cout << "XEnd: " << header.xEnd << endl;
        cout << "YEnd: " << header.yEnd << endl;
        cout << "HorizontalRes: " << header.xDPI << endl;
        cout << "VerticalRes: " << header.yDPI << endl;
        cout << "Reserved1: " << (uint) header.reserved1 << endl;
        cout << "NumBitPlanes: " << (uint) header.numBitPlanes << endl;
        cout << "BytesPerLine: " << header.bytesPerLine << endl;
        cout << "PaletteType: " << header.paletteType << endl;
        cout << "HorizontalScreenSize: " << header.horizontalScreenSize << endl;
        cout << "VerticalScreenSize: " << header.verticalScreenSize << endl;

        cout << "Total header bytes: " << sizeof(PCXHeader) << endl;
    }

    void printEGAPaletteColors() {
        cout << "EGA palette colors: " << endl;
        for (int i = 0; i < 16; i++) {
            cout << "r = " << (uint) egaPaletteColors[i].red
                 << " g = " << (uint) egaPaletteColors[i].green
                 << " b = " << (uint) egaPaletteColors[i].blue << endl;
        }
    }

    void printVGAPaletteColors() {
        cout << "VGA palette colors: " << endl;
        if (vgaPaletteColorsCount <= 0 || vgaPaletteColors == nullptr) {
            cout << "VGA palette is empty " << endl;
        }
        for (int i = 0; i < vgaPaletteColorsCount; i++) {
            cout << "r = " << (uint) vgaPaletteColors[i].red
                 << " g = " << (uint) vgaPaletteColors[i].green
                 << " b = " << (uint) vgaPaletteColors[i].blue << endl;
        }
    }

    Bitmap *toBitmap() {
        auto *bitmap = new Bitmap(width, height);
        if (pixels == nullptr) return bitmap;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int idx = y * width + x;
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

    ~ImagePCX() {
        clear();
    }

    void clear() {
        if (pixels != nullptr) {
            delete[] pixels;
        }
//        if (colors != nullptr) {
//            delete[] colors;
//        }
        pixels = nullptr;
//        colors = nullptr;
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

    void readImage(fstream *fileStream, int bitsPerPixel) {
        if (bitsPerPixel <= 8) {
            readVgaPalette(fileStream, pow(2, bitsPerPixel));
            if (bitsPerPixel == 8 && vgaPaletteColorsCount <= 0) return;
        }
        long imageDataSize = calcImageDataSize(fileStream);
        if (imageDataSize <= 0) return;
        fileStream->seekg(sizeof(PCXHeader), ios_base::beg);

        uint decodedBufferSize = calcScanLineSize();
        auto *decodedBuffer = new uint8_t[decodedBufferSize];
        uint8_t byte;

        long totalDecodedCounter = 0;
        long totalReadedCounter = 0;
        uint readedBytesCounter = 0;
        uint decodedBytesCounter = 0;
        uint rowsCounter = 0;
        pixels = new Color[width*height];

        uint8_t runCount = 0;
        uint8_t runValue = 0;

        while (totalReadedCounter < imageDataSize) {
            readedBytesCounter = 0;
            decodedBytesCounter = 0;
            while (decodedBytesCounter < decodedBufferSize) {
                fileStream->read((char *) &byte, sizeof(byte));
                if ((byte & 0xC0U) == 0xC0U) { //two bytes
                    runCount = byte & 0x3FU;
                    fileStream->read((char *) &byte, sizeof(byte));
                    runValue = byte;
                    readedBytesCounter += 2;
                } else { //one byte
                    runCount = 1;
                    runValue = byte;
                    readedBytesCounter++;
                }
                totalDecodedCounter += runCount;
                for (; runCount > 0 && decodedBytesCounter < decodedBufferSize; decodedBytesCounter++, runCount--) {
                    decodedBuffer[decodedBytesCounter] = runValue;
                }
            }
            totalReadedCounter += readedBytesCounter;
            switch(bitsPerPixel) {
                case 4: bufferTo4BitPixels(decodedBuffer, decodedBufferSize, rowsCounter); break;
                case 8: bufferTo8BitPixels(decodedBuffer, decodedBufferSize, rowsCounter); break;
                case 24: bufferTo24BitPixels(decodedBuffer, decodedBufferSize, rowsCounter); break;
                default: /* Ignore */ break;
            }
            rowsCounter++;
        }
    }

    void bufferTo24BitPixels(const uint8_t *decodedBuffer, uint decodedBufferSize, uint currentRow) {
        uint pixelIdx = 0;
        uint pixelsCount = width * height;
        for (int i = 0; i < header.bytesPerLine; i++) {
            pixelIdx = width * currentRow + i;
            if (pixelIdx < pixelsCount) {
                pixels[pixelIdx].r = decodedBuffer[i];
                pixels[pixelIdx].g = decodedBuffer[i + header.bytesPerLine];
                pixels[pixelIdx].b = decodedBuffer[i + header.bytesPerLine*2];
                pixels[pixelIdx].a = 255;
            }
        }
    }

    void bufferTo8BitPixels(const uint8_t *decodedBuffer, uint decodedBufferSize, uint currentRow) {
        uint scanLinePadding = calcScanLinePadding();
        uint pixelIdx = 0;
        uint pixelsCount = width * height;
        for (int i = 0; i < decodedBufferSize - scanLinePadding; i++) {
            pixelIdx = currentRow * width + i;
            if (pixelIdx < pixelsCount) {
                auto vgaColor = vgaPaletteColors[decodedBuffer[i]];
                pixels[pixelIdx].r = vgaColor.red;
                pixels[pixelIdx].g = vgaColor.green;
                pixels[pixelIdx].b = vgaColor.blue;
                pixels[pixelIdx].a = 255;
            }
        }
    }

    void bufferTo4BitPixels(const uint8_t *decodedBuffer, uint decodedBufferSize, uint currentRow) {
        uint scanLinePadding = calcScanLinePadding();
        uint8_t byte = 0;
        uint pixelIdx = 0;
        uint colorIdx = 0;
        uint pixelsCount = width * height;
        bool useEgaPalette = vgaPaletteColorsCount <= 0;
        for (int i = 0; i < decodedBufferSize - scanLinePadding; i++) {
            byte = decodedBuffer[i];
            for (uint k = 0; k < 2 && i*2+k < width; k++) {
                pixelIdx = currentRow * width + i*2 + k;
                colorIdx = (byte >> (1U-k)*4U) & 0b1111U;
                if (pixelIdx < pixelsCount) {
                    auto currentColor = useEgaPalette ? egaPaletteColors[colorIdx] : vgaPaletteColors[colorIdx];
                    pixels[pixelIdx].r = currentColor.red;
                    pixels[pixelIdx].g = currentColor.green;
                    pixels[pixelIdx].b = currentColor.blue;
                    pixels[pixelIdx].a = 255;
                }
            }
        }
    }

    long calcImageDataSize(fstream *fileStream) {
        fileStream->seekg(0, fstream::end);
        return (fileStream->tellg() - ((long)sizeof(PCXHeader)) - (long(sizeof(PCX24Color) * vgaPaletteColorsCount)));
    }

    uint calcScanLineSize() {
        return header.numBitPlanes * header.bytesPerLine;
    }

    uint calcScanLinePadding() {
        return ((header.bytesPerLine * header.numBitPlanes) * (8 / header.bitsPerPixel)) - width;
    }

    void readVgaPalette(fstream *fileStream, int colorsCount) {
        vgaPaletteColorsCount = 0;
        vgaPaletteColors = nullptr;
        uint8_t byte = 0;
        int vgaPaletteSize = (int) (sizeof(PCX24Color) * colorsCount);
        fileStream->seekg(-vgaPaletteSize-1, fstream::end); // -1 to check one more byte
        fileStream->read((char *) &byte, sizeof(byte));
        if (byte == 12) { //palette exists
            vgaPaletteColorsCount = colorsCount;
            vgaPaletteColors = new PCX24Color[colorsCount];
            fileStream->read((char *) vgaPaletteColors, vgaPaletteSize);
        }
    }
//
//    void readImage(fstream *fileStream) {
//        pixels = new Color[width * height];
//        int additionalBytesToRead = calcPadding(4);
//        uint8_t byte = 0;
//        uint pixelIdx = 0;
//        BMPPaletteItem paletteColor{};
//        uint8_t colorIndex = 0;
//        uint8_t bitsPerPixel = 4;
//        uint8_t mask = 0B1111;
//
//        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);
//        for (int i = height - 1; i >= 0; i--) {
//            for (int j = 0; j < width;) {
//                fileStream->read((char *) &byte, sizeof(byte));
//                for (uint k = 0; k < 2; k++) {
//                    colorIndex = mask & (byte >> (k * bitsPerPixel));
//                    pixelIdx = i * width + j;
//                    if (colorIndex < paletteColorsCount) {
//                        paletteColor = colors[colorIndex];
//                        pixels[pixelIdx].r = paletteColor.red;
//                        pixels[pixelIdx].g = paletteColor.green;
//                        pixels[pixelIdx].b = paletteColor.blue;
//                    }
//                    j++;
//                }
//            }
//            for (int k = 0; k < additionalBytesToRead; k++) {
//                fileStream->read((char *) &byte, sizeof(byte));
//            }
//        }
//    }
//
//    void readColor1Image(fstream *fileStream) {
//        pixels = new Color[width * height];
//        int additionalBytesToRead = calcPadding(1);
//        int8_t byte;
//        uint pixelIdx = 0;
//        BMPPaletteItem paletteColor{};
//        uint8_t colorIndex = 0;
//        uint8_t bitsPerPixel = 1;
//        uint8_t mask = 0B1;
//
//        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);
//        for (int i = height - 1; i >= 0; i--) {
//            for (int j = 0; j < width;) {
//                fileStream->read((char *) &colorIndex, sizeof(colorIndex));
//                for (uint k = 0; k < 8; k++) {
//                    colorIndex = mask & (colorIndex >> (k * bitsPerPixel));
//                    pixelIdx = i * width + j;
//                    if (colorIndex < paletteColorsCount) {
//                        paletteColor = colors[colorIndex];
//                        pixels[pixelIdx].r = paletteColor.red;
//                        pixels[pixelIdx].g = paletteColor.green;
//                        pixels[pixelIdx].b = paletteColor.blue;
//                    }
//                    j++;
//                }
//            }
//            for (int k = 0; k < additionalBytesToRead; k++) {
//                fileStream->read((char *) &byte, sizeof(byte));
//            }
//        }
//    }
//
//    void writeColor32Image(fstream *fileStream) {
//        int additionalBytesPerRow = calcPadding(32);
//        writeHeaders(fileStream, 32, additionalBytesPerRow);
//        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);
//
//        int idx = 0;
//        int8_t byte = 0;
//        BMP32Color color;
//        for (int i = height - 1; i >= 0; i--) {
//            for (int j = 0; j < width; j++) {
//                idx = i * width + j;
//                color.red = pixels[idx].r;
//                color.green = pixels[idx].g;
//                color.blue = pixels[idx].b;
//                color.alpha = pixels[idx].a;
//                fileStream->write((char *) &color, sizeof(BMP32Color));
//            }
//            for (int k = 0; k < additionalBytesPerRow; k++) {
//                fileStream->write((char *) &byte, sizeof(byte));
//            }
//        }
//    }
//
//    void writeColor24Image(fstream *fileStream) {
//        int additionalBytesPerRow = calcPadding(24);
//        writeHeaders(fileStream, 24, additionalBytesPerRow);
//        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);
//
//        int idx = 0;
//        int8_t byte = 0;
//        BMP24Color color;
//        for (int i = height - 1; i >= 0; i--) {
//            for (int j = 0; j < width; j++) {
//                idx = i * width + j;
//                color.red = pixels[idx].r;
//                color.green = pixels[idx].g;
//                color.blue = pixels[idx].b;
//                fileStream->write((char *) &color, sizeof(BMP24Color));
//            }
//            for (int k = 0; k < additionalBytesPerRow; k++) {
//                fileStream->write((char *) &byte, sizeof(byte));
//            }
//        }
//    }
//
//    void writeColor8Image(fstream *fileStream) {
//        int additionalBytesPerRow = calcPadding(8);
//        paletteColorsCount = 256;
//        writeHeaders(fileStream, 8, additionalBytesPerRow);
//
//        Palette palette(pixels, width * height);
//        cout << "Change colors count form " << palette.colors->size() << " to " << paletteColorsCount << "..." << endl;
//        MedianSection median;
//        palette.resize(paletteColorsCount, median);
//        writePalette(fileStream, palette);
//
//        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);
//        int idx = 0;
//        int8_t byte = 0;
//        uint8_t paletteIndex = 0;
//        Color currentPixel;
//        for (int i = height - 1; i >= 0; i--) {
//            for (int j = 0; j < width; j++) {
//                idx = i * width + j;
//                currentPixel = pixels[idx];
//                paletteIndex = palette.getNearestIndex(currentPixel);
//                fileStream->write((char *) &paletteIndex, sizeof(paletteIndex));
//            }
//            for (int k = 0; k < additionalBytesPerRow; k++) {
//                fileStream->write((char *) &byte, sizeof(byte));
//            }
//        }
//    }
//
//    void writeColor4Image(fstream *fileStream) {
//        int additionalBytesPerRow = calcPadding(4);
//        paletteColorsCount = 16;
//        writeHeaders(fileStream, 4, additionalBytesPerRow);
//
//        Palette palette(pixels, width * height);
//        cout << "Change colors count form " << palette.colors->size() << " to " << paletteColorsCount << "..." << endl;
//        MedianSection median;
//        palette.resize(paletteColorsCount, median);
//
//        writePalette(fileStream, palette);
//        fileStream->seekg(fileHeader.bfOffsetBits, ios_base::beg);
//
//        int idx = 0;
//        uint8_t byte = 0;
//        uint8_t paletteIndex = 0;
//        Color currentPixel;
//        uint8_t mask = 0B1111;
//        uint8_t bitsPerPixel = 4;
//        for (int i = height - 1; i >= 0; i--) {
//            for (int j = 0; j < width;) {
//                byte = 0;
//                for (int k = 0; k < 2 && j < width; k++) {
//                    idx = i * width + j;
//                    currentPixel = pixels[idx];
//                    paletteIndex = palette.getNearestIndex(currentPixel);
//                    byte = byte | ((uint)(mask & paletteIndex) << (uint)((2 - k - 1) * bitsPerPixel));
//                    j++;
//                }
//                fileStream->write((char *) &byte, sizeof(byte));
//            }
//            byte = 0;
//            for (int k = 0; k < additionalBytesPerRow; k++) {
//                fileStream->write((char *) &byte, sizeof(byte));
//            }
//        }
//    }

//    void writeHeader(fstream *fileStream) {
//        int pixelsCount = width * height;
//        fileHeader.bfType = BF_TYPE;
//        fileHeader.bfSize = sizeof(fileHeader)
//                            + sizeof(imageHeader)
//                            + sizeof(BMPPaletteItem) * paletteColorsCount
//                            + pixelsCount * bitsPerPixel / 8
//                            + additionalBytesPerRow * height;
//        fileHeader.bfOffsetBits = sizeof(fileHeader)
//                                  + sizeof(imageHeader)
//                                  + sizeof(BMPPaletteItem) * paletteColorsCount;
//        fileHeader.bfReserved1 = 0;
//        fileHeader.bfReserved2 = 0;
//        imageHeader.size = sizeof(BMPImageHeader);
//        imageHeader.width = width;
//        imageHeader.height = height;
//        imageHeader.planesCount = 1;
//        imageHeader.bitsPerPixel = bitsPerPixel;
//        imageHeader.compressionType = 0;
//        imageHeader.imageSize = (bitsPerPixel * pixelsCount) / 8 + additionalBytesPerRow * height;
//        imageHeader.xPixelsPerMeter = 0;
//        imageHeader.yPixelsPerMeter = 0;
//        imageHeader.colorsCount = paletteColorsCount;
//        imageHeader.colorsImportantCount = paletteColorsCount;
//        fileStream->write((char *) &fileHeader, sizeof(BMPFileHeader));
//        fileStream->write((char *) &imageHeader, sizeof(BMPImageHeader));
//    }
//
//    void writePalette(fstream *fileStream, Palette &palette) {
//        BMPPaletteItem paletteItem{};
//        for (auto color: *palette.colors) {
//            paletteItem.red = color.r;
//            paletteItem.green = color.g;
//            paletteItem.blue = color.b;
//            paletteItem.reversed = color.a;
//            fileStream->write((char *) &paletteItem, sizeof(BMPPaletteItem));
//        }
//    }


#pragma pack(push, 1)
    struct PCXHeader {
        int8_t identifier;        /* PCX Id Number (Always 0x0A) */
        int8_t version;           /* Version Number */
        int8_t encoding;          /* Encoding Format */
        int8_t bitsPerPixel;      /* Bits per Pixel */
        int16_t xStart;            /* Left of image */
        int16_t yStart;            /* Top of Image */
        int16_t xEnd;              /* Right of Image */
        int16_t yEnd;              /* Bottom of image */
        int16_t xDPI;           /* Horizontal Resolution */
        int16_t yDPI;           /* Vertical Resolution */
        int8_t palette[48];       /* 16-Color EGA Palette */
        int8_t reserved1;         /* Reserved (Always 0) */
        int8_t numBitPlanes;      /* Number of Bit Planes */
        int16_t bytesPerLine;      /* Bytes per Scan-line */
        int16_t paletteType;       /* Palette Type */
        int16_t horizontalScreenSize;    /* Horizontal Screen Size */
        int16_t verticalScreenSize;    /* Vertical Screen Size */
        int8_t reserved2[54];     /* Reserved (Always 0) */
    } header;
#pragma pack(pop)

#pragma pack(push, 1)
    struct PCX24Color {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } *egaPaletteColors = nullptr;
#pragma pack(pop)


    PCX24Color *vgaPaletteColors = nullptr;
    int vgaPaletteColorsCount = 0;

};


#endif //IMAGEANALYSER_IMAGEPCX_H
