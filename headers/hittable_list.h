#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "common.h"
#include "hittable.h"
#include "aabb.h"
#include <vector>

class hittable_list : public hittable {
private:
    aabb bbox;

public:
    std::vector<shared_ptr<hittable>> objs;
    double area_sum;

    hittable_list(){}
    hittable_list(shared_ptr<hittable> obj) {add(obj);}

    void clear(){ objs.clear(); bbox = aabb(); }
    void add(shared_ptr<hittable> obj){
        objs.push_back(obj);
        bbox = aabb(bbox, obj->bounding_box());
    }

    bool hit(const ray& r, interval t_int, hit_record& hr) const {
        hit_record tmpRec;
        bool hitAnything = false;
        interval workingInterval = interval(t_int.min, t_int.max);

        for(const shared_ptr<hittable>& obj : objs){
            if(obj->hit(r, workingInterval, tmpRec)){
                hitAnything = true;
                workingInterval.max =tmpRec.t;
                hr = tmpRec;
            }
        }

        return hitAnything;
    }


    // TODO perhaps take normal into account to know what ovjects are actually seen

    double pdf_value(const point3& origin, const vec3& direction) const {
        int index = rand_int(0, objs.size());
        return objs[index]->pdf_value(origin, direction) * objs[index]->area_facing(direction) / area_facing(direction);
    }

    double area_facing(const vec3& direction) const { // returns the area projected according to the direction
        double sum = 0;
        for(auto obj : objs)
            sum += obj->area_facing(direction);
        return sum;
    }

    point3 random_point() const {
        int index = rand_int(0, objs.size());
        return objs[index]->random_point();
    }

    point3 random_point_towards(const point3& position) const  {
        int index = rand_int(0, objs.size());
        return objs[index]->random_point_towards(position);
    }
    point3 random_point_facing(const vec3& direction) const {
        int index = rand_int(0, objs.size());
        return objs[index]->random_point_facing(direction);
    }

    aabb bounding_box() const override {
        return bbox;
    }
};





#endif