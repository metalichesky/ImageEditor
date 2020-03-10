//
// Created by dmitriy on 09.03.2020.
//

#ifndef IMAGEANALYSER_LISTENERS_H
#define IMAGEANALYSER_LISTENERS_H

#include "../ui/View.h"
#include <functional>

class Listeners {
    typedef function<void(View*, sf::Event)> OnClickListener;
};

#endif //IMAGEANALYSER_LISTENERS_H
