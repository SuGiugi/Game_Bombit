//
// Created by Admin on 3/15/2025.
//

#ifndef LOGIC_H
#define LOGIC_H

class Logic {
public:
    int up(double a, double b);
    int down(double a, double b);
    int min(const int &a, const int &b) const {return (a<b?a:b);};
    int max(const int &a, const int &b) const {return (a>b?a:b);};
    double round_2(double a);
};

#endif //LOGIC_H
