//
// Created by dmitriy on 19.03.2020.
//

#include "ImagePCX.h"

const int8_t ImagePCX::PCX_ID = 0x0A;
const int8_t ImagePCX::PCX_VERSION0 = 0; //Version 2.5 with fixed EGA palette information
const int8_t ImagePCX::PCX_VERSION1 = 1;
const int8_t ImagePCX::PCX_VERSION2 = 2; //Version 2.8 with modifiable EGA palette information
const int8_t ImagePCX::PCX_VERSION3 = 3; //Version 2.8 without palette information
const int8_t ImagePCX::PCX_VERSION4 = 4; //PC Paintbrush for Windows
const int8_t ImagePCX::PCX_VERSION5 = 5; //Version 3.0 of PC Paintbrush, PC Paintbrush Plus, PC Paintbrush Plus for Windows, Publisher's Paintbrush, and all 24-bit image files

const int8_t ImagePCX::PCX_ENCODING_NONE = 0; //disabled encoding
const int8_t ImagePCX::PCX_ENCODING_RLE = 1; //simple byte-wise run-length encoding scheme

const int8_t ImagePCX::PCX_RESERVED1 = 0; //default reserved value

const int8_t ImagePCX::PCX_PALETTE_TYPE0 = 0; //ignoring
const int8_t ImagePCX::PCX_PALETTE_TYPE1 = 1; //color, monochrome
const int8_t ImagePCX::PCX_PALETTE_TYPE2 = 2; //grey-scaled