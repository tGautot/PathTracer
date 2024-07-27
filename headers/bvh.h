// Bounding Volume Hierarchy
#ifndef BVH_H
#define BVH_H

#include <memory>
#include <algorithm>
#include <iostream>

#include "hittable.h"
#include "hittable_list.h"


using std::shared_ptr;

bool box_x_compare(shared_ptr<hittable> a, shared_ptr<hittable> b){
    return a->bounding_box().x.min < b->bounding_box().x.min;
}
bool box_y_compare(shared_ptr<hittable> a, shared_ptr<hittable> b){
    return a->bounding_box().y.min < b->bounding_box().y.min;
}
bool box_z_compare(shared_ptr<hittable> a, shared_ptr<hittable> b){
    return a->bounding_box().z.min < b->bounding_box().z.min;
}

class bvh_node : public hittable {
private:
    shared_ptr<hittable> left, right; // children in the tree structure
    aabb bbox;

public:

    // hl not by ref because we want a temporary copy
    bvh_node(hittable_list hl): bvh_node(hl.objs, 0, hl.objs.size()){}

    bvh_node(std::vector<shared_ptr<hittable>>& objs, size_t stt, size_t end){
        bbox = aabb(interval::empty,interval::empty,interval::empty);
        for (size_t object_index=stt; object_index < end; object_index++)
            bbox = aabb(bbox, objs[object_index]->bounding_box());

        int axis = bbox.longest_axis();
        auto compare_func = (axis==0) ? box_x_compare :
                            (axis==1) ? box_y_compare : box_z_compare;

        size_t span = end-stt;

        if(span == 1){
            left = right = objs[stt];
        } else if(span == 2){
            left = objs[stt];
            right = objs[stt+1];    
        } else {
            std::sort(objs.begin() + stt, objs.begin() + end, compare_func);
            size_t mid = (stt+end)/2;
            left = make_shared<bvh_node>(objs, stt, mid);
            right = make_shared<bvh_node>(objs, mid, end);
        }
        bbox = aabb(left->bounding_box(), right->bounding_box());
        
    }

    bool hit(const ray& r, interval ray_t, hit_record& hr) const override {

        if(!bbox.hit(r, ray_t)){
            return false;
        }
        
        
        bool hitLeft  =  left->hit(r, ray_t, hr);
        bool hitRight = right->hit(r, interval(ray_t.min, hitLeft ? hr.t : ray_t.max), hr);
        return hitLeft || hitRight;
    }

    aabb bounding_box() const override {
        return bbox;
    }

};



#endif 