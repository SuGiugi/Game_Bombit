//
// Created by Admin on 3/15/2025.
//
#include <cmath>
#include "logic.h"

int Logic::up(const double a,const double b) {
    if (a < b) return static_cast<int>(floor(a));
    return static_cast<int>(ceil(a));
}

int Logic::down(const double a,const double b) {
    if (a < b) return static_cast<int>(ceil(a));
    return static_cast<int>(floor(a));
}


double Logic::round_2(double a) {
    return round(a*10)/10;
};