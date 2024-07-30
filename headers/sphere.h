#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"
#include "hittable.h"
#include "material.h"
#include "time_profiler.h"

class sphere : public hittable{
private:
    aabb bbox;
public:
    point3 center;
    double radius;
    shared_ptr<material> mat;

    sphere(const point3 c, double r, shared_ptr<material> m): center(c), radius(r), mat(m) {
        bbox = aabb(center - radius*vec3(1,1,1), center + radius*vec3(1,1,1));
    }
    //sphere(const point3& c, double r, shared_ptr<material> m): 
    //    center(c), radius(r), mat(m) {}

    aabb bounding_box() const override {
        return bbox;
    }

    bool hit(const ray& r, interval t_int, hit_record& hr) const{
        // axx + bx + c
        vec3 dir = vec3(r.direction());
        vec3 toSphereDir = center - r.origin();
        double a = dot(dir, dir);
        double h = dot(dir, toSphereDir); // h = b/-2
        double c = dot(toSphereDir, toSphereDir) - radius*radius;
        double delta = h*h - a*c;
        
        if(delta < 0){ 
            return false;
        }

        double sqrtDelta = sqrt(delta);
        double root = (h - sqrtDelta)/a;
        if(!t_int.surrounds(root)){
            root = (h+sqrtDelta)/a;
            if(!t_int.surrounds(root)){
                return false;
            }
        }

        hr.t = root;
        hr.p = r.at(root);
        hr.normal = (hr.p - center) / radius;
        get_sphere_uv(hr.normal,hr.u,hr.v);
        
        if(dot(hr.normal, r.direction()) > 0.0){
            // ray is inside the sphere
            hr.front_face = false;
            hr.normal = - hr.normal;    
        } else {
            hr.front_face = true;
        }
        hr.mat = mat;

        return true; 
    }
    
    
    static void get_sphere_uv(const point3& p, double& u, double& v) {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = std::acos(-p.y());
        auto phi = std::atan2(-p.z(), p.x()) + PI;

        u = phi / (2*PI);
        v = theta / PI;
    }
};



#endif