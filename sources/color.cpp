#include "color.h"

void write_color(std::ostream& out, const color& c){
    double r = linear_to_gamma(c.x());
    double g = linear_to_gamma(c.y());
    double b = linear_to_gamma(c.z());

    int ir = int(255.999 * r);
    int ig = int(255.999 * g);
    int ib = int(255.999 * b);

    out << ir << ' ' << ig << ' ' << ib << '\n';
}