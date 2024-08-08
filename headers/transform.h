#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vec3.h"
#include "mat3.h"
#include <vector>

class transform {

private:
    mat3 basis;

public: 
    point3 center;

    transform(): center() {
        basis = mat3::idt();
    }
    transform(const point3& c): center(c) {
        basis = mat3::idt();
    }

    void set_basis(int axis, vec3 v){
        basis.e[axis] = v;
    }


    vec3& get_basis(int axis) {
        //std::clog << "Transform returning reference to axis " << axis << " address is " << std::addressof(basis.e[axis]) << std::endl;
        return basis.e[axis];
    }

    point3& get_center() {
        return center;
    }

    void translate(const vec3& offset){
        center += offset;
    }

    void rotate(double degX, double degY, double degZ){
        this->rotate_around(degX, degY, degZ, center);
    }

    virtual void rotate_around(double degX, double degY, double degZ, const point3& pivot) {
        //std::clog << "In transform's rotate_around(), basis before rotation: " << std::endl;
        //basis.print();
        //std::clog << "Basis first col address is " << std::addressof(basis.e[0]) << std::endl;
        mat3 rot = mat3::idt();

        if(std::fabs(degX) > EPSILON){
            double radX = degX * PI/180.0;
            mat3 rotX = mat3::rot_x(radX); 
            rot = rot.multiply(rotX);
        }
        if(std::fabs(degY) > EPSILON){
            double radY = degY * PI/180.0;
            mat3 rotY = mat3::rot_y(radY); 
            rot = rot.multiply(rotY);
        }
        if(std::fabs(degZ) > EPSILON){
            double radZ = degZ * PI/180.0;
            mat3 rotZ = mat3::rot_z(radZ);  
            rot = rot.multiply(rotZ);   
        }

        basis = rot.multiply(basis);
        //std::clog << "Basis after rotation: " << std::endl;
        //basis.print();

        if(&center != &pivot)
            center = pivot + rot.multiply(center-pivot);
    }
};

#endif