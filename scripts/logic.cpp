//
// Created by Admin on 3/15/2025.
//
#include <cmath>
#include "logic.h"

int Logic::up(double a, double b) {
    if (a < b) return static_cast<int>(floor(a));
    return static_cast<int>(ceil(a));
}

int Logic::down(double a, double b) {
    if (a < b) return static_cast<int>(ceil(a));
    return static_cast<int>(floor(a));
}

float Logic::after_mark(float a) {
    return a - static_cast<float>(static_cast<int>(a));
}

double Logic::round_2(double a) {
    return round(a*10)/10;
};
