//
// Created by dmitriy on 18.03.2020.
//

#ifndef IMAGEANALYSER_BITMAPCODER_H
#define IMAGEANALYSER_BITMAPCODER_H


#include <bitset>
#include <cstring>
#include "CoderData.h"
#include "../Bitmap.h"

class BitmapCoder {
public:
    static CoderData decodeFrom(Bitmap *bitmap) {
        CoderData data;
        int pixelsCount = bitmap->width * bitmap->height;
        int maxBytesCount = floor(((float) pixelsCount * 3.0f) / 8.0f);
        int sizeFieldSize = sizeof(data.size);
        auto *sizeData = new uint8_t[sizeFieldSize];
        data.size = 0;
        data.bytes = nullptr;
        uint8_t byte = 0;
        int bitsCounter = 0;
        int bytesCounter = 0;
        uint8_t bitMask = (uint) 0b1;
        uint8_t currentBit = 0;
        for (int i = 0; i < pixelsCount; i++) {
            for (int j = 0; j < 3; j++) {
                currentBit = bitmap->getPixel(i)->getComponent(j) & bitMask;
                byte = byte | (uint) (currentBit << (uint) (7 - (bitsCounter % 8)));
                bitsCounter++;
                if (bitsCounter % 8 == 0) {
                    bitsCounter = 0;
                    bytesCounter++;
                    if (bytesCounter > maxBytesCount) {
                        return data;
                    }
                    if (bytesCounter > sizeFieldSize) {
                        if (bytesCounter - sizeFieldSize > data.size) {
                            return data;
                        }
                        data.bytes[bytesCounter - sizeFieldSize - 1] = byte;
                    } else if (bytesCounter == sizeFieldSize) {
                        copy(reinterpret_cast<char *>(sizeData),
                             reinterpret_cast<char *>(sizeData) + sizeFieldSize,
                             reinterpret_cast<char *>(&data.size)
                        );
                        if (data.size > 0) {
                            data.bytes = new uint8_t[data.size];
                        } else {
                            return data;
                        }
                    } else {
                        sizeData[bytesCounter - 1] = byte;
                    }
                    byte = 0;
                }
            }
        }
        return data;
    }

    static void encodeTo(Bitmap *bitmap, CoderData data) {
        if (data.size <= 0) return;
        int pixelsCount = bitmap->width * bitmap->height;
        int bytesEnabledCount = floor(((float) pixelsCount * 3.0f) / 8.0f);
        int sizeFieldSize = sizeof(data.size);
        auto *sizeData = new uint8_t[sizeFieldSize];
        copy(reinterpret_cast<char *>(&data.size), reinterpret_cast<char *>(&data.size) + sizeFieldSize, sizeData);

        uint8_t currentByte = sizeData[0];
        int bitsCounter = 0;
        int bytesCounter = 0;
        uint8_t currentBit = 0;
        for (int i = 0; i < pixelsCount; i++) {
            for (int j = 0; j < 3; j++) {
                uint8_t bitMask = 0b1U << (7U - (bitsCounter % 8U));
                currentBit = currentByte & bitMask;
                uint8_t component = bitmap->getPixel(i)->getComponent(j);
                component = (currentBit != 0) ? (component | 0b1U) : (component & (~0b1U));
                bitmap->getPixel(i)->setComponent(j, component);
                cout << "Write bit " << bitset<8>(currentBit) << " component was " << bitset<8>(component) << " now "
                     << bitset<8>(bitmap->getPixel(i)->getComponent(j)) << endl;
                bitsCounter++;
                if (bitsCounter % 8 == 0) {
                    cout << "Byte " << bitset<8>(currentByte) << endl;
                    bitsCounter = 0;
                    bytesCounter++;
                    if (bytesCounter >= sizeFieldSize) {
                        currentByte = data.bytes[bytesCounter - sizeFieldSize];
                    } else {
                        currentByte = sizeData[bytesCounter];
                    }
                    if (bytesCounter >= (data.size + sizeFieldSize) || bytesCounter >= bytesEnabledCount) return;

                }
            }
        }
    }

};


#endif //IMAGEANALYSER_BITMAPCODER_H
