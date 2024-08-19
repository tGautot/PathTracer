    #ifndef HITTABLE_H
#define HITTABLE_H

#include "common.h"
#include "aabb.h"

class material;
class hittable;

class hit_record{
public:
    point3 p;
    vec3 normal;
    double t;
    double u, v, w;
    bool front_face;
    shared_ptr<material> mat;
    shared_ptr<hittable> target; // only filled when calling `hit` function on `hittable_list`


    void set_frontface_and_normal(const ray& r, const vec3 outnrml){
        front_face = dot(r.direction(), outnrml) <= 0.0;
        normal = front_face ? outnrml : -outnrml;
    }
};

class hittable {
public:
    virtual ~hittable() = default;

    virtual void commit_transform() = 0;

    virtual bool hit(const ray& r, interval t_int, hit_record& hr) const = 0;

    virtual aabb bounding_box() const = 0;

    virtual double pdf_value(const point3& origin, const vec3& direction) const {
        return 0;
    }

    virtual double get_area() const { // returns the area
        return 0;
    }

    virtual bool is_facing(const vec3& dir) const {
        return false;
    }

    virtual point3 random_point() const {
        return point3(0,0,0);
    };

    virtual point3 random_point_towards(const point3& position) const {
        return point3(0,0,0);
    };
    
};


#endif
