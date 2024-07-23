#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

using color = vec3;

inline double linear_to_gamma(double linearComponent) {
    if (linearComponent > 0)
        return sqrt(linearComponent);

    return 0;
}

void write_color(std::ostream& out, const color& c);



#endif