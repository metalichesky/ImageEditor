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
#include "../Bitmap.h"
#include "ImageFile.h"
#include "ImageFormat.h"
#include "../palette/Palette.h"

using namespace std;

class ImagePCX : public ImageFile {
    typedef void(ImagePCX::*BufferToPixelsFunction)(const uint8_t *, uint, uint);

    typedef void(ImagePCX::*PixelsToBufferFunction)(uint8_t *, uint, uint, Palette *);

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
    static const int16_t PCX_DPI_DEFAULT;
    static const int8_t PCX_RESERVED1;
    static const int16_t PCX_PALETTE_TYPE0;
    static const int16_t PCX_PALETTE_TYPE1;
    static const int16_t PCX_PALETTE_TYPE2;
    static const int16_t PCX_HORIZONTAL_SCREEN_DEFAULT;
    static const int16_t PCX_VERTICAL_SCREEN_DEFAULT;

    void readFromFile(string filePath) override {
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

        readImage(&fileStream);

        fileStream.close();

        printEGAPaletteColors();
        printVGAPaletteColors();
        printHeader();
    }

    void writeToFile(string filePath, ImageFormat format) override {
        cout << "Writing PCX image file " + filePath << endl;
        fstream fileStream;
        fileStream.open(filePath, fstream::out | fstream::binary);

        writeImage(&fileStream, (uint) (format.colorDepth));

        fileStream.close();

        printEGAPaletteColors();
        printVGAPaletteColors();
        printHeader();
    }

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
        if (egaPaletteColors != nullptr) {
            for (int i = 0; i < 16; i++) {
                cout << "r = " << (uint) egaPaletteColors[i].red
                     << " g = " << (uint) egaPaletteColors[i].green
                     << " b = " << (uint) egaPaletteColors[i].blue << endl;
            }
        } else {
            cout << "EGA palette is empty" << endl;
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
        delete[] vgaPaletteColors;
        delete[] egaPaletteColors;
    }


private:
    void readImage(fstream *fileStream) {
        uint bitPerColor = calcBitPerColor();
        if (bitPerColor <= 8) {
            readVgaPalette(fileStream, pow(2, bitPerColor));
            if (bitPerColor == 8 && vgaPaletteColorsCount <= 0) return;
        }
        long imageDataSize = calcImageDataSize(fileStream);
        if (imageDataSize <= 0) return;
        fileStream->seekg(sizeof(PCXHeader), ios_base::beg);

        uint decodedBufferSize = calcScanLineSize();
        auto *decodedBuffer = new uint8_t[decodedBufferSize];
        uint8_t byte;

        BufferToPixelsFunction bufferToPixelsFunc;
        switch (bitPerColor) {
            case 1:
                bufferToPixelsFunc = &ImagePCX::bufferTo1BitPixels;
                break;
            case 2:
                bufferToPixelsFunc = &ImagePCX::bufferTo2BitPixels;
                break;
            case 4:
                bufferToPixelsFunc = &ImagePCX::bufferTo4BitPixels;
                break;
            case 8:
                bufferToPixelsFunc = &ImagePCX::bufferTo8BitPixels;
                break;
            case 24:
                bufferToPixelsFunc = &ImagePCX::bufferTo24BitPixels;
                break;
            default:
                return;/* Ignore */
        }

        long totalDecodedCounter = 0;
        long totalReadedCounter = 0;
        uint readedBytesCounter = 0;
        uint decodedBytesCounter = 0;
        uint rowsCounter = 0;
        pixels = new Color[width * height];

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
            (this->*bufferToPixelsFunc)(decodedBuffer, decodedBufferSize, rowsCounter);
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
                pixels[pixelIdx].b = decodedBuffer[i + header.bytesPerLine * 2];
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
            for (uint k = 0; k < 2 && i * 2 + k < width; k++) {
                pixelIdx = currentRow * width + i * 2 + k;
                colorIdx = (byte >> (1U - k) * 4U) & 0b1111U;
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

    void bufferTo2BitPixels(const uint8_t *decodedBuffer, uint decodedBufferSize, uint currentRow) {
        uint scanLinePadding = calcScanLinePadding();
        uint8_t byte = 0;
        uint pixelIdx = 0;
        uint colorIdx = 0;
        uint pixelsCount = width * height;
        for (int i = 0; i < decodedBufferSize - scanLinePadding; i++) {
            byte = decodedBuffer[i];
            for (uint k = 0; k < 4 && i * 4 + k < width; k++) {
                pixelIdx = currentRow * width + i * 4 + k;
                colorIdx = (byte >> (3U - k) * 2U) & 0b11U;
                if (pixelIdx < pixelsCount) {
                    auto currentColor = egaPaletteColors[colorIdx];
                    pixels[pixelIdx].r = currentColor.red;
                    pixels[pixelIdx].g = currentColor.green;
                    pixels[pixelIdx].b = currentColor.blue;
                    pixels[pixelIdx].a = 255;
                }
            }
        }
    }

    void bufferTo1BitPixels(const uint8_t *decodedBuffer, uint decodedBufferSize, uint currentRow) {
        uint scanLinePadding = calcScanLinePadding();
        uint8_t byte = 0;
        uint pixelIdx = 0;
        uint colorIdx = 0;
        uint pixelsCount = width * height;
        for (int i = 0; i < decodedBufferSize - scanLinePadding; i++) {
            byte = decodedBuffer[i];
            for (uint k = 0; k < 8 && i * 8 + k < width; k++) {
                pixelIdx = currentRow * width + i * 8 + k;
                colorIdx = (byte >> (7U - k)) & 0b1U;
                if (pixelIdx < pixelsCount) {
                    auto currentColor = egaPaletteColors[colorIdx];
                    pixels[pixelIdx].r = currentColor.red;
                    pixels[pixelIdx].g = currentColor.green;
                    pixels[pixelIdx].b = currentColor.blue;
                    pixels[pixelIdx].a = 255;
                }
            }
        }
    }

    void pixels24BitToBuffer(uint8_t *lineBuffer, uint lineBufferSize, uint currentRow, Palette *palette) {
        uint scanLinePadding = calcScanLinePadding();
        uint pixelIdx = 0;
        uint colorIdx = 0;
        uint pixelsCount = width * height;
        for (int i = 0; i < header.bytesPerLine; i++) {
            pixelIdx = currentRow * width + i;
            if (pixelIdx < pixelsCount) {
                lineBuffer[i] = pixels[pixelIdx].r;
                lineBuffer[i+header.bytesPerLine] = pixels[pixelIdx].g;
                lineBuffer[i+header.bytesPerLine*2] = pixels[pixelIdx].b;
            }
        }
    }

    void pixels8BitToBuffer(uint8_t *lineBuffer, uint lineBufferSize, uint currentRow, Palette *palette) {
        uint scanLinePadding = calcScanLinePadding();
        uint8_t byte = 0;
        uint pixelIdx = 0;
        uint colorIdx = 0;
        uint pixelsCount = width * height;
        for (int i = 0; i < lineBufferSize - scanLinePadding; i++) {
            byte = 0;
            pixelIdx = currentRow * width + i;
            if (pixelIdx < pixelsCount) {
                colorIdx = palette->getNearestIndex(&pixels[pixelIdx]);
                byte = colorIdx;
            }
            lineBuffer[i] = byte;
        }
    }


    void pixels4BitToBuffer(uint8_t *lineBuffer, uint lineBufferSize, uint currentRow, Palette *palette) {
        uint scanLinePadding = calcScanLinePadding();
        uint8_t byte = 0;
        uint pixelIdx = 0;
        uint colorIdx = 0;
        uint pixelsCount = width * height;
        for (int i = 0; i < lineBufferSize - scanLinePadding; i++) {
            byte = 0;
            for (uint k = 0; k < 2 && i * 2 + k < width; k++) {
                pixelIdx = currentRow * width + i * 2 + k;
                if (pixelIdx < pixelsCount) {
                    colorIdx = palette->getNearestIndex(&pixels[pixelIdx]);
                    byte = byte | ((colorIdx & 0b1111U) << (1U - k)*4);
                }
            }
            lineBuffer[i] = byte;
        }
    }

    void pixels2BitToBuffer(uint8_t *lineBuffer, uint lineBufferSize, uint currentRow, Palette *palette) {
        uint scanLinePadding = calcScanLinePadding();
        uint8_t byte = 0;
        uint pixelIdx = 0;
        uint colorIdx = 0;
        uint pixelsCount = width * height;
        for (int i = 0; i < lineBufferSize - scanLinePadding; i++) {
            byte = 0;
            for (uint k = 0; (k < 4) && ((i * 4 + k) < width); k++) {
                pixelIdx = currentRow * width + i * 4 + k;
                if (pixelIdx < pixelsCount) {
                    colorIdx = palette->getNearestIndex(&pixels[pixelIdx]);
                    byte = byte | ((colorIdx & 0b11U) << (3U - k)*2U);
                }
            }
            lineBuffer[i] = byte;
        }
    }

    void pixels1BitToBuffer(uint8_t *lineBuffer, uint lineBufferSize, uint currentRow, Palette *palette) {
        uint scanLinePadding = calcScanLinePadding();
        uint8_t byte = 0;
        uint pixelIdx = 0;
        uint colorIdx = 0;
        uint pixelsCount = width * height;
        for (int i = 0; i < lineBufferSize - scanLinePadding; i++) {
            byte = 0;
            for (uint k = 0; k < 8 && i * 8 + k < width; k++) {
                pixelIdx = currentRow * width + i * 8 + k;
                if (pixelIdx < pixelsCount) {
                    colorIdx = palette->getNearestIndex(&pixels[pixelIdx]);
                    byte = byte | ((colorIdx & 0b1U) << (7U - k));
                }
            }
            lineBuffer[i] = byte;
        }
    }

    uint calcNumBitPlanes(uint bitPerColor) {
        uint numBitPlanes = 1;
        switch (bitPerColor) {
            case 3 :
                numBitPlanes = 3;
                break;
            case 24:
                numBitPlanes = 3;
                break;
        }
        return numBitPlanes;
    }

    uint calcBitPerColor() {
        return header.bitsPerPixel * header.numBitPlanes;
    }

    long calcImageDataSize(fstream *fileStream) {
        fileStream->seekg(0, fstream::end);
        return (fileStream->tellg() - ((long) sizeof(PCXHeader)) - (long(sizeof(PCX24Color) * vgaPaletteColorsCount)));
    }

    uint calcScanLineSize() {
        return header.numBitPlanes * header.bytesPerLine;
    }

    uint calcScanLinePadding() {
        return ((header.numBitPlanes * header.bytesPerLine) * (8 / header.bitsPerPixel)) - width;
    }

    void readVgaPalette(fstream *fileStream, int colorsCount) {
        vgaPaletteColorsCount = 0;
        vgaPaletteColors = nullptr;
        uint8_t byte = 0;
        int vgaPaletteSize = (int) (sizeof(PCX24Color) * colorsCount);
        fileStream->seekg(-vgaPaletteSize - 1, fstream::end); // -1 to check one more byte
        fileStream->read((char *) &byte, sizeof(byte));
        if (byte == 12) { //palette exists
            vgaPaletteColorsCount = colorsCount;
            vgaPaletteColors = new PCX24Color[colorsCount];
            fileStream->read((char *) vgaPaletteColors, vgaPaletteSize);
        }
    }

    void writeImage(fstream *fileStream, uint bitsPerColor) {
        Palette* palette = preparePalette(bitsPerColor);
        writeHeader(fileStream, palette, bitsPerColor);


        PixelsToBufferFunction pixelsToBufferFunc;
        switch (bitsPerColor) {
            case 1:
                pixelsToBufferFunc = &ImagePCX::pixels1BitToBuffer;
                break;
            case 2:
                pixelsToBufferFunc = &ImagePCX::pixels2BitToBuffer;
                break;
            case 4:
                pixelsToBufferFunc = &ImagePCX::pixels4BitToBuffer;
                break;
            case 8:
                pixelsToBufferFunc = &ImagePCX::pixels8BitToBuffer;
                break;
            case 24:
                pixelsToBufferFunc = &ImagePCX::pixels24BitToBuffer;
                break;
            default:
                return;/* Ignore */
        }

        uint lineBufferSize = calcScanLineSize();
        auto *lineBuffer = new uint8_t[lineBufferSize];
        uint8_t byte;

        uint8_t runCount = 0;
        uint8_t runValue = 0;
        uint8_t currentValue = 0;

        for (int i = 0; i < height; i++) {
            (this->*pixelsToBufferFunc)(lineBuffer, lineBufferSize, i, palette);
            runCount = 0;
            runValue = 0;
            for (int j = 0; j < lineBufferSize; j++) {
                currentValue = lineBuffer[j];
                if (runCount == 0) {
                    runValue = currentValue;
                }
                if (runValue == currentValue) {
                    runCount++;
                    if (runCount == 0x3FU) {//0x3F = 62 = 0b00111111
                        byte = runCount | 0xC0U;
                        fileStream->write((char*) &byte, sizeof(byte));
                        byte = runValue;
                        fileStream->write((char*) &byte, sizeof(byte));
                        runCount = 0;
                    }
                } else if (runCount > 0) {
                    if (runCount > 1 || (runValue & 0xC0U) == 0xC0U) {
                        byte = runCount | 0xC0U;
                        fileStream->write((char *) &byte, sizeof(byte));
                        byte = runValue;
                        fileStream->write((char *) &byte, sizeof(byte));
                    } else {
                        byte = runValue;
                        fileStream->write((char *) &byte, sizeof(byte));
                    }
                    runValue = currentValue;
                    runCount = 1;
                }
            }
            if (runCount > 0) {
                if (runCount == 1 && (runValue & 0xC0U) != 0xC0U) {
                    byte = runValue;
                    fileStream->write((char*) &byte, sizeof(byte));
                } else {
                    byte = runCount | 0xC0U;
                    fileStream->write((char*) &byte, sizeof(byte));
                    byte = runValue;
                    fileStream->write((char*) &byte, sizeof(byte));
                }
            }
        }

        if (isNeedAnyPalette(bitsPerColor) && isNeedVgaPalette(bitsPerColor)) {
            writePaletteVGA(fileStream, palette);
        }
    }

    bool isNeedAnyPalette(uint bitPerColor) {
        return bitPerColor <= 8;
    }

    bool isNeedVgaPalette(uint bitPerColor) {
        return bitPerColor > 4;
    }

    Palette* preparePalette(uint bitsPerColor) {
        Palette *palette = nullptr;
        if (isNeedAnyPalette(bitsPerColor)) {
            palette = new Palette(pixels, width * height);
            int destColors = pow(2, bitsPerColor);
            MedianSection median;
            palette->resize(destColors, median);
        }
        return palette;
    }

    void writeHeader(fstream *fileStream, Palette *palette, uint bitsPerColor) {
        if (isNeedAnyPalette(bitsPerColor) && !isNeedVgaPalette(bitsPerColor)) {
            uint egaPaletteColorsCount = 16;
            egaPaletteColors = new PCX24Color[egaPaletteColorsCount];
            for (int i = 0; i < egaPaletteColorsCount; i++) {
                if (i < palette->colors->size()) {
                    egaPaletteColors[i].red = palette->colors->at(i).r;
                    egaPaletteColors[i].green = palette->colors->at(i).g;
                    egaPaletteColors[i].blue = palette->colors->at(i).b;
                } else {
                    egaPaletteColors[i].red = 0;
                    egaPaletteColors[i].green = 0;
                    egaPaletteColors[i].blue = 0;
                }
            }
            copy(reinterpret_cast<char *>(egaPaletteColors),
                 reinterpret_cast<char *>(egaPaletteColors) + sizeof(PCX24Color) * egaPaletteColorsCount,
                 header.palette);
        }
        uint numBitPlanes = calcNumBitPlanes(bitsPerColor);
        header.identifier = PCX_ID;
        header.version = PCX_VERSION5;
        header.encoding = PCX_ENCODING_RLE;
        header.bitsPerPixel = bitsPerColor / numBitPlanes;
        header.xStart = 0;
        header.yStart = 0;
        header.xEnd = width - 1;
        header.yEnd = height - 1;
        header.xDPI = PCX_DPI_DEFAULT;
        header.yDPI = PCX_DPI_DEFAULT;
        header.reserved1 = PCX_RESERVED1;
        header.numBitPlanes = numBitPlanes;
        header.bytesPerLine = (uint) ceil((float) (bitsPerColor * width) / 8.0f);
        header.paletteType = PCX_PALETTE_TYPE1;
        header.horizontalScreenSize = PCX_HORIZONTAL_SCREEN_DEFAULT;
        header.verticalScreenSize = PCX_VERTICAL_SCREEN_DEFAULT;
        fileStream->write((char *) &header, sizeof(PCXHeader));
    }

    Palette *writePaletteVGA(fstream *fileStream, Palette *palette) {
        PCX24Color paletteItem{};
        vgaPaletteColorsCount = palette->colors->size();
        vgaPaletteColors = new PCX24Color[vgaPaletteColorsCount];
        for (int i = 0; i < vgaPaletteColorsCount; i++) {
            vgaPaletteColors[i].red = palette->colors->at(i).r;
            vgaPaletteColors[i].green = palette->colors->at(i).g;
            vgaPaletteColors[i].blue = palette->colors->at(i).b;
        }
        uint8_t vgaFlagByte = 12U;
        fileStream->write((char *) &vgaFlagByte, sizeof(vgaFlagByte));
        fileStream->write((char *) vgaPaletteColors, sizeof(PCX24Color) * vgaPaletteColorsCount);
        return palette;
    }


#pragma pack(push, 1)
    struct PCXHeader {
        uint8_t identifier;        /* PCX Id Number (Always 0x0A) */
        uint8_t version;           /* Version Number */
        uint8_t encoding;          /* Encoding Format */
        uint8_t bitsPerPixel;      /* Bits per Pixel */
        uint16_t xStart;            /* Left of image */
        uint16_t yStart;            /* Top of ImageFile */
        uint16_t xEnd;              /* Right of ImageFile */
        uint16_t yEnd;              /* Bottom of image */
        uint16_t xDPI;           /* Horizontal Resolution */
        uint16_t yDPI;           /* Vertical Resolution */
        uint8_t palette[48];       /* 16-Color EGA Palette */
        uint8_t reserved1;         /* Reserved (Always 0) */
        uint8_t numBitPlanes;      /* Number of Bit Planes */
        uint16_t bytesPerLine;      /* Bytes per Scan-line */
        uint16_t paletteType;       /* Palette Type */
        uint16_t horizontalScreenSize;    /* Horizontal Screen Size */
        uint16_t verticalScreenSize;    /* Vertical Screen Size */
        uint8_t reserved2[54];     /* Reserved (Always 0) */
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


//
//enum class PCXType {
//    COLOR1BIT, //NumBitPlanes 1, BitsPerPixel 1, Monochrome
//    COLOR2BIT, //NumBitPlanes 1, BitsPerPixel 2, CGA
//    COLOR3BIT, //NumBitPlanes 3, BitsPerPixel 1, EGA //not supported yet
//    COLOR4BIT, //NumBitPlanes 1, BitsPerPixel 4, EGA and VGA
//    COLOR8BIT, //NumBitPlanes 1, BitsPerPixel 8, Extended VGA
//    COLOR24BIT //NumBitPlanes 3, BitsPerPixel 8, Extended VGA and XGA
//};


#endif //IMAGEANALYSER_IMAGEPCX_H
