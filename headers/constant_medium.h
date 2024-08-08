#ifndef CONSTANT_MDEIUM_H
#define CONSTANT_MDEIUM_H

#include "box.h"
#include "material.h"

class constant_medium : public hittable {
private:
    shared_ptr<hittable> boundary;
    double neg_inv_density;
    shared_ptr<material> phase_func;

public:
    constant_medium(shared_ptr<hittable> boundary, double density, shared_ptr<texture> tex)
      : boundary(boundary), neg_inv_density(-1/density),
        phase_func(make_shared<isotropic>(tex))
    {}

    constant_medium(shared_ptr<hittable> boundary, double density, const color& albedo)
      : boundary(boundary), neg_inv_density(-1/density),
        phase_func(make_shared<isotropic>(albedo))
    {}

    bool hit(const ray& r, interval t_int, hit_record& hr) const override {
        hit_record hr1, hr2;
        
        // Ray origin might be in volume, need to be careful
        if(!boundary->hit(r, interval::universe, hr1)) return false;
        if(!boundary->hit(r, interval(hr1.t+0.001, infinity), hr2)) return false;


        hr1.t = std::fmax(hr1.t, t_int.min);
        hr2.t = std::fmin(hr2.t, t_int.max);

        if(hr2.t < hr1.t) return false;

        double raySpeed = r.direction().length();
        double hitDist = (hr2.t -hr1.t) * raySpeed;
        double escapeDist = neg_inv_density * std::log(randDouble());

        if(hitDist < escapeDist) return false;
        
        // Hit in volume confirmed

        hr.t = hr1.t + escapeDist/raySpeed;
        hr.p = r.at(hr.t);
        hr.mat = phase_func;
        hr.normal = vec3(1,0,0);
        hr.front_face = true;

        return true;
    };

    aabb bounding_box() const override { return boundary->bounding_box(); }
};

#endif