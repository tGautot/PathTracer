#ifndef INTERVAL_H
#define INTERVAL_H

#include "common.h"

class interval {
public:
    double min, max;

    interval(): min(+infinity), max(-infinity){}
    interval(double l, double u): min(l), max(u){}
    interval(const interval& a, const interval& b) {
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

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

    interval expand(double delta){
        double pad = delta/2.0;
        return interval(min-pad, max+pad);
    }

    inline double size() const{
        return max-min;
    }

    static const interval empty, universe;

};


#endif