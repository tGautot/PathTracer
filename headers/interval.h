#ifndef INTERVAL_H
#define INTERVAL_H

#include "common.h"

class interval {
public:
    double min, max;

    interval(): min(+infinity), max(-infinity){}
    interval(double l, double u): min(l), max(u){}

    bool contains(double x){
        return min <= x && x <= max;
    }

    bool surrounds(double x){
        return min < x && x < max;
    }

    double clamp(double x){
        if(x < min) return min;
        if(x > max) return max;
        return x;
    }

    static const interval empty, universe;

};


#endif