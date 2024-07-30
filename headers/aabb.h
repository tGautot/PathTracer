#ifndef AABB_H
#define AABB_H

#include "interval.h"
#include "time_profiler.h"

// Axis Aligned Bounding Boxes
class aabb {
public:
    interval x, y, z;

    aabb(){}
    aabb(const interval& x, const interval& y, const interval& z): x(x), y(y), z(z) {}
    aabb(const point3& a, const point3& b){
        x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
        y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
        z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);
    }
    aabb(const aabb& box0, const aabb& box1) {
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }

    void expand(double delta = 0.001){
        x.expand(delta);
        y.expand(delta);
        z.expand(delta);
    }

    interval axis_interval(int n) const{
        if(n == 1) return y;
        if(n == 2) return z;
        return x;
    }

    bool hit(const ray& r, interval rayIntrvl) const{
        const vec3   rayDir = r.direction();
        const point3 rayPos = r.origin();
    
        interval ax;
        for(int axis = 0; axis < 3; axis++){
            ax = axis_interval(axis);

            double t0 = (ax.min - rayPos[axis]) / rayDir[axis];
            double t1 = (ax.max - rayPos[axis]) / rayDir[axis]; 
        
            if(t1 < t0) swap(&t0, &t1);
            if(t0 > rayIntrvl.min) rayIntrvl.min = t0;
            if(t1 < rayIntrvl.max) rayIntrvl.max = t1;

            if(rayIntrvl.min >= rayIntrvl.max) {
                return false;
            }
        }
        
        return true;
    }

    int longest_axis() const {
        // Returns the index of the longest axis of the bounding box.

        if (x.size() > y.size())
            return x.size() > z.size() ? 0 : 2;
        else
            return y.size() > z.size() ? 1 : 2;
    }

    std::ostream& to_stream(std::ostream& out) const{
        return out << "((" << x.min << "," << x.max << "), (" << y.min  << "," << y.max << "), (" << z.min  << ","  << z.max << "))";
    }
};



#endif