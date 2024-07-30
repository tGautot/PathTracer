#ifndef QUADS_H
#define QUADS_H

#include "hittable.h"
#include "aabb.h"


// abstract class for all 2D shapes
class planar_shape: public hittable {

protected:
    // parallelogram defined by the plane spanned by vec u and v
    // when originating a orig
    point3 q;
    vec3 u, v;
    
    vec3 n;

public:

    planar_shape(point3 q, vec3 u, vec3 v): q(q), u(u), v(v){}

    void planar_coordinates(const point3& p, double* alpha, double* beta) const {
        by_system_solving(p, alpha, beta);
    }

    bool ray_plane_intersection(const ray& r, double* hitTime) const{
        // N dot (P + tD - Q) = 0
        double nd = dot(n, r.direction());

        // if only want to render fronface, want nd negative only
        if(std::fabs(nd) <= EPSILON) return false;
        
        double npq = dot(n, r.origin() - q);
        double tm  = npq/nd;
        *hitTime = tm;
        return true;
    }

    virtual bool is_interior(double alpha, double beta, hit_record& hr) const = 0;

private:
    void by_system_solving(const point3& p, double* ka, double* kb) const {
        // Ray hit plane (at C) defined by u,v now check if in parallelogram
        // C = Q + ka*v + kb * u

        double eq1[3], eq2[3];
        

        if(v[0] <= EPSILON && u[0] <= EPSILON){
            eq1[0] = p[1] - q[1];
            eq1[1] = v[1];
            eq1[2] = u[1];

            eq2[0] = p[2] - q[2];
            eq2[1] = v[2];
            eq2[2] = u[2];
        } else {
            eq1[0] = p[0] - q[0];
            eq1[1] = v[0];
            eq1[2] = u[0];
            if(v[1] <= EPSILON && u[1] <= EPSILON){
                eq2[0] = p[2] - q[2];
                eq2[1] = v[2];
                eq2[2] = u[2];
            } else {
                eq2[0] = p[1] - q[1];
                eq2[1] = v[1];
                eq2[2] = u[1];
            }
        } 


        solve_decomposition(eq1, eq2, ka, kb);
    }

    void solve_decomposition(double* eq1, double* eq2, double *ka, double *kb) const {
        // eq1 and eq2 should contain r v u from r = ka*v + kb*u
        // ka and kb are output parameters

        if(std::fabs(eq1[1]) <= EPSILON){
            solve_decomposition(eq2, eq1, ka, kb);
            return;
        }  

        *kb = (eq2[0] - (eq2[1]/eq1[1])*eq1[0])/(eq1[2]+eq2[2]);
        *ka = (eq1[0] - *kb*eq1[2])/eq1[1];
    }

};

class quad : public planar_shape {
private:

    
    shared_ptr<material> mat;
    aabb bbox;


public:
    quad(point3 q, vec3 u, vec3 v, shared_ptr<material> mat): planar_shape(q,u,v), mat(mat){     
        n = cross(u,v);
        aabb bbox1 = aabb(q, q+u+v);
        aabb bbox2 = aabb(q+u, q+v);
        bbox = aabb(bbox1, bbox2);
        bbox.expand(); // avoid problems with axis aligned quads
    }

    bool hit(const ray& r, interval t_int, hit_record& hr) const override{
        //std::cout << "Checking for hit with quat at " << q << ", normal is " << n << std::endl;
        double hitTime;
        if(!ray_plane_intersection(r, &hitTime)) return false;
        point3 hitPoint = r.at(hitTime);
        //std::cout << "Found hit with plane at " << hitPoint << std::endl;
        double ka, kb;
        planar_coordinates(hitPoint, &ka, &kb);
        
        if(!is_interior(ka, kb, hr)) return false;

        hr.t = hitTime;
        hr.p = hitPoint;
        hr.mat = mat;
        hr.normal = n;
        hr.front_face = dot(r.direction(), n) <= 0.0;

        //std::cout << "FrontFace computed via normal: " << n  << std::endl;

        return true;
    }

    bool is_interior(double alpha, double beta, hit_record& hr) const override {
        interval oi = interval(0.0, 1.0);

        if(!oi.contains(alpha) || !oi.contains(beta)) return false;

        hr.u = beta; hr.v = alpha;
        return true;
    }

    aabb bounding_box() const override {
        return bbox;
    }

};

#endif