//
// Created by dmitriy on 22.03.2020.
//

#ifndef IMAGEANALYSER_IMAGE_H
#define IMAGEANALYSER_IMAGE_H

#include "ImageFile.h"
#include "ImageFormat.h"
#include "ImagePCX.h"
#include "ImageBMP.h"

class Image {
public:
    static ImageFile* createFileObject(ImageFormat &format) {
        ImageFile *imageFile = nullptr;
        switch(format.type) {
            case FormatType::BMP :
                imageFile = new ImageBMP();
                break;
            case FormatType::PCX :
                imageFile = new ImagePCX();
                break;
        }
        return imageFile;
    }
};


#endif //IMAGEANALYSER_IMAGE_H
