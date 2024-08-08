#ifndef QUADS_H
#define QUADS_H

#include "transform.h"
#include "hittable.h"
#include "aabb.h"


// abstract class for all 2D shapes
class planar_shape: public hittable, public transform {

protected:
    // parallelogram defined by the plane spanned by vec u and v
    // when originating a orig
    point3 &q = get_center();
    vec3 &u = get_basis(0), &v = get_basis(1), &n = get_basis(2);
    
    vec3 normal;

    double D;
    vec3 w;

public:

    planar_shape(point3 q, vec3 uvec, vec3 vvec): transform(q){
        u.set(uvec); v.set(vvec);
        n = cross(u,v);
        normal = n.normalized();
        D = dot(q, normal);
        w = n / dot(n,n);
    }

    void translate(const vec3& offset)  {
        transform::translate(offset);
        D = dot(q, normal);
    }

    void rotate_around(double degX, double degY, double degZ, const point3& pivot) {
        transform::rotate_around(degX, degY, degZ, pivot);
        w = n / dot(n,n);
        
        // Should not be needed, but might be worth putting it in when debugging
        // normal = n.normalized();
    }



    void planar_coordinates(const point3& p, double* alpha, double* beta) const {
        by_vector_formula(p, alpha, beta);
    }

    bool ray_plane_intersection(const ray& r, double* hitTime) const{
        // N dot (P + tD - Q) = 0
        double nd = dot(normal, r.direction());

        // if only want to render fronface, want nd negative only
        if(std::fabs(nd) <= EPSILON) return false;
        
        double npq = D - dot(normal, r.origin());
        //std::clog << "Computing intersection time from " << npq << "/" << nd << std::endl;
        double tm  = npq/nd;
        *hitTime = tm;
        return true;
    }

    virtual bool is_interior(double alpha, double beta, hit_record& hr) const = 0;

private:
    void by_vector_formula(const point3& p, double* ka, double* kb) const {
        *ka = dot(w, cross(p-q, v));
        *kb = dot(w, cross(u, p-q));
    }

    void by_system_solving(const point3& p, double* ka, double* kb) const {
        // Ray hit plane (at C) defined by u,v now check if in parallelogram
        // C = Q + kb*v + ka * u

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

        *ka = (eq2[0] - (eq2[1]/eq1[1])*eq1[0])/(eq1[2]+eq2[2]);
        *kb = (eq1[0] - *ka*eq1[2])/eq1[1];
    }

};

class quad : public planar_shape {
private:

    
    shared_ptr<material> mat;
    aabb bbox;


public:
    quad(point3 q, vec3 u, vec3 v, shared_ptr<material> mat): planar_shape(q,u,v), mat(mat){     
        aabb bbox1 = aabb(q, q+u+v);
        aabb bbox2 = aabb(q+u, q+v);
        bbox = aabb(bbox1, bbox2);
        bbox.expand(); // avoid problems with axis aligned quads
    }

    bool hit(const ray& r, interval t_int, hit_record& hr) const override{
        //std::clog << "Checking for hit with quat at " << q << ", normal is " << normal << std::endl;
        double hitTime;
        if(!ray_plane_intersection(r, &hitTime)) return false;
        point3 hitPoint = r.at(hitTime);
        //std::clog << "Found hit with plane at " << q << " at time " << hitTime << " hit pos is " << hitPoint << std::endl;
        
        if(!t_int.contains(hitTime)) return false;
        double ka, kb;
        planar_coordinates(hitPoint, &ka, &kb);
        
        //std::clog << "Planar coordinates are " << ka << ", " << kb << std::endl;

        if(!is_interior(ka, kb, hr)) return false;

        hr.t = hitTime;
        hr.p = hitPoint;
        hr.mat = mat;
        hr.set_frontface_and_normal(r, normal);
        

        //std::clog << "Hit IN parallelogrom, normal: " << normal << ", ff: " << hr.front_face  << std::endl;

        return true;
    }

    bool is_interior(double alpha, double beta, hit_record& hr) const override {
        interval oi = interval(0.0, 1.0);

        if(!oi.contains(alpha) || !oi.contains(beta)) return false;

        hr.u = alpha; hr.v = beta;
        return true;
    }

    aabb bounding_box() const override {
        return bbox;
    }

};

#endif