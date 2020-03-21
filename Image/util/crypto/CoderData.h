//
// Created by dmitriy on 18.03.2020.
//

#ifndef IMAGEANALYSER_CODERDATA_H
#define IMAGEANALYSER_CODERDATA_H


#include <cstdint>
#include <string>
#include <fstream>

using namespace std;

class CoderData {
public:
    long size = 0;
    uint8_t *bytes = nullptr;
    CoderData() = default;
    CoderData(int newSize, uint8_t *newBytes) : size(newSize), bytes(newBytes) {}

    void readFromFile(string filePath) {
        fstream fileStream;
        fileStream.open(filePath, fstream::in | fstream::binary | ios::ate);
        long fileSize = fileStream.tellg();
        size = fileSize;
        bytes = new uint8_t[fileSize];
        fileStream.seekg(0, fstream::beg);
        fileStream.read((char *) bytes, sizeof(uint8_t) * fileSize);
        fileStream.close();
    }

    void writeToFile(string filePath) {
        fstream fileStream;
        fileStream.open(filePath, fstream::out | fstream::binary);
        fileStream.write((char *) bytes, sizeof(uint8_t) * size);
        fileStream.close();
    }

    ~CoderData(){
    }
};


#endif //IMAGEANALYSER_CODERDATA_H
