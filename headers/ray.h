#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
private:
    point3 orig;
    vec3 dir;
    double tm; // time at which the ray was sent

public:
    ray() {}

    ray(const point3& origin, const vec3& direction): orig(origin), dir(direction), tm(0) {}
    ray(const point3& origin, const vec3& direction, double time): orig(origin), dir(direction), tm(time) {}

    const point3& origin() const {return orig;}
    const vec3& direction() const {return dir;}
    double time() const {return tm;}

    point3 at(double t) const{
        return orig + t*dir;
    }
    friend std::ostream& operator<<(std::ostream& out, const ray& r);
};


#endif