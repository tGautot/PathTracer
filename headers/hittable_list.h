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


    void commit_transform() override{
#ifdef SIMPLE_DEBUG
        std::clog << "Committed list transform" << std::endl;
#endif
        for(auto obj : objs){
            obj->commit_transform();
        }
    };

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
                hr.target = obj;
            }
        }

        return hitAnything;
    }


    // TODO perhaps take normal into account to know what ovjects are actually seen

    

    aabb bounding_box() const override {
        return bbox;
    }
};





#endif