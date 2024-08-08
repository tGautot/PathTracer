#ifndef BOX_H
#define BOX_H

#include "transform.h"
#include "vec3.h"
#include "hittable_list.h"
#include "shape2d.h"
#include "mat3.h"

class box : public hittable_list, public transform {
private:
    vec3 &dx= get_basis(0), &dy= get_basis(1), &dz= get_basis(2);
    point3 min, max;
    shared_ptr<material> mat;
    

public:
    box(const point3& a, const point3& b, shared_ptr<material> mat): transform(), mat(mat){

        // Construct the two opposite vertices with the minimum and maximum coordinates.
        min = point3(std::fmin(a.x(),b.x()), std::fmin(a.y(),b.y()), std::fmin(a.z(),b.z()));
        max = point3(std::fmax(a.x(),b.x()), std::fmax(a.y(),b.y()), std::fmax(a.z(),b.z()));

        center = (min+max)/2;

        //dx = get_basis(0); dy = get_basis(1); dz = get_basis(2); 

        //std::clog << "Address of box dx is " << std::addressof(dx[0]) << std::endl;

        dx.e[0] = max.x() - min.x();
        dy.e[1] = max.y() - min.y();
        dz.e[2] = max.z() - min.z();

        build_faces();
    }

    void build_faces(){
        add(make_shared<quad>(min,  dx,  dy, mat)); // back
        add(make_shared<quad>(min,  dx,  dz, mat)); // bottom
        add(make_shared<quad>(min,  dz,  dy, mat)); // left
        add(make_shared<quad>(max, -dx, -dy, mat)); // front
        add(make_shared<quad>(max, -dz, -dy, mat)); // right
        add(make_shared<quad>(max, -dx, -dz, mat)); // top
    }

    void translate(const vec3& offset)  {
        transform::translate(offset);
        min = center - (dx + dy + dz)/2;
        max = min + dx + dy + dz;
        clear(); build_faces();
    }

    void rotate_around(double degX, double degY, double degZ, const point3& pivot) {
        //std::clog << "BOX BEFORE ROTATE " << std::endl;
        //std::clog << "Address of box dx is " << std::addressof(dx[0]) << std::endl;
        //print();

        transform::rotate_around(degX, degY, degZ, pivot);
        min = center - (dx + dy + dz)/2;;
        max = min + dx + dy + dz;
        clear(); build_faces();
        
        //std::clog << "BOX AFTER ROTATE " << std::endl;
        //std::clog << "Address of box dx is " << std::addressof(dx[0]) << std::endl;
        //print();
    }

    void print(){
        std::clog << "dx: " << dx << "\ndy: " << dy << "\ndz: " << dz << "\nmin: " << min << "\nmax: " << max << std::endl; 
    }



};



#endif