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

        //build_faces();
    }

    void commit_transform(){
        min = center - (dx + dy + dz)/2;
        max = min + dx + dy + dz;
        build_faces();
    }

    void build_faces(){
        add(make_shared<quad>(min,  dx,  dy, mat, true)); // back
        add(make_shared<quad>(min,  dz,  dx, mat, true)); // bottom
        add(make_shared<quad>(min,  dy,  dz, mat, true)); // left
        add(make_shared<quad>(max, -dy, -dx, mat, true)); // front
        add(make_shared<quad>(max, -dz, -dy, mat, true)); // right
        add(make_shared<quad>(max, -dx, -dz, mat, true)); // top
    }

    /*void translate(const vec3& offset)  {
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
    }*/

   double pdf_value(const point3& origin, const vec3& direction) const {
        //Generate uniformly by surface over all 6 planes
        
        
        std::vector<double> cumsum;
        double totArea = 0;
        hittable_list faces;
        for(auto obj : objs) {
            if(!obj->is_facing(direction)) continue;
            faces.add(obj);
            totArea += obj->get_area();
        }
    

        bool hit_smthg = false;
        hit_record hr;


        if(!faces.hit(ray(origin, direction), interval(0.001, infinity), hr)) {
            return 0;
        }

        shared_ptr<hittable> hit_obj;
        for(auto obj: faces.objs){
            if(obj == hr.target){
                return obj->pdf_value(origin, direction) * obj->get_area() / totArea;
            }
                
        }

        std::clog << "REACHED UNEXPECTED RETURN IN box.h " << __LINE__ << std::endl;
        return 0;
    }

    double area_facing(const vec3& direction) const { // returns the area projected according to the direction
        double sum = 0;
        for(auto obj : objs){
            if(is_facing(direction))
                sum += obj->get_area();
        }
        return sum;
    }

    point3 random_point() const {
        std::vector<double> cumsum;
        for(int i = 0; i < objs.size(); i++) {
            auto obj = objs[i];
            double obj_area = obj->get_area();
            if(cumsum.size() == 0) cumsum.push_back(cumsum.back() + obj_area);
        }
        double r = randDouble(0, cumsum.back());
        int index = std::lower_bound(cumsum.begin(), cumsum.end(), r) - cumsum.begin();

        return objs[index]->random_point();
    }

    point3 random_point_towards(const point3& position) const  {
        vec3 dir = center-position;
        std::vector<double> cumsum;
        for(int i = 0; i < objs.size(); i++) {
            auto obj = objs[i];
            if(!obj->is_facing(dir)) continue;
            double obj_area = obj->get_area();
            if(cumsum.size() == 0) cumsum.push_back(cumsum.back() + obj_area);
        }
        double r = randDouble(0, cumsum.back());
        int index = std::lower_bound(cumsum.begin(), cumsum.end(), r) - cumsum.begin();

        return objs[index]->random_point();
    }

    void print(){
        std::clog << "dx: " << dx << "\ndy: " << dy << "\ndz: " << dz << "\nmin: " << min << "\nmax: " << max << std::endl; 
    }



};



#endif