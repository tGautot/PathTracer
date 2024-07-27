#ifndef HITTABLE_H
#define HITTABLE_H

#include "common.h"
#include "aabb.h"

class material;

class hit_record{
public:
    point3 p;
    vec3 normal;
    double t;
    double u, v;
    bool front_face;
    shared_ptr<material> mat;
};

class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval t_int, hit_record& hr) const = 0;

    virtual aabb bounding_box() const = 0;
};


#endif
