#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>


using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;
const double EPSILON = 1e-8;


inline double degsToRads(double deg){
    return deg * PI / 180.0;
}

inline double radsToDeg(double rads){
    return rads * 180.0 / PI;
}

inline double randDouble(){
    return rand() / (RAND_MAX+1.0);
}

inline double randDouble(double min, double max){
    return min + randDouble()*(max-min);
}



#endif