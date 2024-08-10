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

    void set_frontface_and_normal(const ray& r, const vec3 outnrml){
        front_face = dot(r.direction(), outnrml) <= 0.0;
        normal = front_face ? outnrml : -outnrml;
    }
};

class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval t_int, hit_record& hr) const = 0;

    virtual aabb bounding_box() const = 0;

    virtual double pdf_value(const point3& origin, const vec3& direction) const {
        return 0;
    }

    virtual double area_facing(const vec3& direction) const { // returns the area projected according to the direction
        return 0;
    }

    virtual point3 random_point() const {
        return point3(0,0,0);
    };

    virtual point3 random_point_towards(const point3& position) const {
        return point3(0,0,0);
    };
    virtual point3 random_point_facing(const vec3& direction) const {
        return point3(0,0,0);
    };
};


#endif
