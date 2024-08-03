#ifndef BOX_H
#define BOX_H

#include "vec3.h"
#include "hittable_list.h"
#include "shape2d.h"
#include "mat3.h"

class box : public hittable_list {
private:
    vec3 dx, dy, dz;
    point3 min, max;
    shared_ptr<material> mat;
    
    point3 center;

public:
    box(const point3& a, const point3& b, shared_ptr<material> mat): mat(mat){

        // Construct the two opposite vertices with the minimum and maximum coordinates.
        min = point3(std::fmin(a.x(),b.x()), std::fmin(a.y(),b.y()), std::fmin(a.z(),b.z()));
        max = point3(std::fmax(a.x(),b.x()), std::fmax(a.y(),b.y()), std::fmax(a.z(),b.z()));

        center = (min+max)/2;

        dx = vec3(max.x() - min.x(), 0, 0);
        dy = vec3(0, max.y() - min.y(), 0);
        dz = vec3(0, 0, max.z() - min.z());

        build_faces();
    }

    void build_faces(){
        add(make_shared<quad>(point3(min.x(), min.y(), max.z()),  dx,  dy, mat)); // front
        add(make_shared<quad>(point3(max.x(), min.y(), max.z()), -dz,  dy, mat)); // right
        add(make_shared<quad>(point3(max.x(), min.y(), min.z()), -dx,  dy, mat)); // back
        add(make_shared<quad>(point3(min.x(), min.y(), min.z()),  dz,  dy, mat)); // left
        add(make_shared<quad>(point3(min.x(), max.y(), max.z()),  dx, -dz, mat)); // top
        add(make_shared<quad>(point3(min.x(), min.y(), min.z()),  dx,  dz, mat)); // bottom
    }

    void rotate(double degX, double degY, double degZ){
        std::clog << "Box before rotation" << std::endl;
        print();
        mat3 rot = mat3::idt();

        if(std::fabs(degX) > EPSILON){
            double radX = degX * 180/PI;
            mat3 rotX = mat3::rot_x(radX); 
            rot = rot.multiply(rotX);
        }
        if(std::fabs(degY) > EPSILON){
            double radY = degY * 180/PI;
            mat3 rotY = mat3::rot_y(radY); 
            rot = rot.multiply(rotY);
        }
        if(std::fabs(degZ) > EPSILON){
            double radZ = degZ * 180/PI;
            mat3 rotZ = mat3::rot_z(radZ);  
            rot = rot.multiply(rotZ);   
        }

        std::clog << "Rotation matrix: " << std::endl;
        rot.print();

        dx = rot.multiply(dx);
        dy = rot.multiply(dy);
        dz = rot.multiply(dz);

        min = center + rot.multiply(min-center);
        max = center + rot.multiply(max-center);

        std::clog << "Box AFTER rotation" << std::endl;
        print();

        clear();
        build_faces();
    }

    void translate(const vec3& offset){
        min += offset;
        max += offset;
        center += offset;
        clear();
        build_faces();
    }

    void print(){
        std::clog << "dx: " << dx << "\ndy: " << dy << "\ndz: " << dz << "\nmin: " << min << "\nmax: " << max << std::endl; 
    }



};



#endif