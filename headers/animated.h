#ifndef ANIMATED_H
#define ANIMATED_H

#include <type_traits>
#include "hittable.h"
#include "interval.h"

template <typename T> class animated : public hittable {
private:
    void (*animate_to)(double, T&);
    const T default_obj;
public:

    static_assert(std::is_base_of<hittable, T>::value, "Only hittable objects can be animated");

    animated(void (*anim_func)(double, T&)): animate_to(anim_func), default_obj(T()){} 
    animated(void (*anim_func)(double, T&), const T&& dflt): animate_to(anim_func), default_obj(dflt){}

    bool hit(const ray& r, interval t_int, hit_record& hr) const override {
        T obj_at_time = T(default_obj);
        animate_to(r.time(), obj_at_time);
        return obj_at_time.hit(r, t_int, hr);
    }

};


#endif