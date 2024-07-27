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

    hittable_list(){}
    hittable_list(shared_ptr<hittable> obj) {add(obj);}

    void clear(){ objs.clear(); }
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

    aabb bounding_box() const override {
        return bbox;
    }
};





#endif