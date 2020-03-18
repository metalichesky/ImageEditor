//
// Created by dmitriy on 14.03.2020.
//

#ifndef IMAGEANALYSER_COMPARATOR_H
#define IMAGEANALYSER_COMPARATOR_H

template <class T>
class Comparator {
public:
    virtual int compare(T first, T second) {
        int result = 0;
        if (first > second) {
            result = 1;
        } else if (first < second) {
            result = -1;
        }
        return result;
    }
};


#endif //IMAGEANALYSER_COMPARATOR_H
