//
// Created by dmitriy on 14.03.2020.
//

#ifndef IMAGEANALYSER_SORTER_H
#define IMAGEANALYSER_SORTER_H

#include "Comparator.h"

template <class T>
class Sorter {
public:
    explicit Sorter()  = default;
    virtual void sort(T *arr, int size){};
};


#endif //IMAGEANALYSER_SORTER_H
