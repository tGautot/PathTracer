#ifndef MAT3_H
#define MAT3_H

#include <cmath>
#include "vec3.h"

class mat3{
public:
    vec3 e[3]; // cols

    mat3(){} // All 0

    mat3(vec3 e0, vec3 e1, vec3 e2){
        e[0] = e0; e[1] = e1; e[2] = e2;
        //e[0][0] = e0[0]; e[0][1] = e0[1]; e[0][2] = e0[2]; // col0
        //e[1][0] = e1[0]; e[1][1] = e1[1]; e[1][2] = e1[2]; // col1
        //e[2][0] = e2[0]; e[2][1] = e2[2]; e[2][2] = e2[2]; // col2
    }

    static mat3 rot_x(double rads){
        vec3 cols[3];
        cols[0] = vec3(1,0,0);
        cols[1] = vec3(0,cos(rads),sin(rads));
        cols[2] = vec3(0,-sin(rads),cos(rads));
        return mat3(cols[0], cols[1], cols[2]);
    }

    static mat3 rot_y(double rads){
        vec3 cols[3];
        cols[0] = vec3(cos(rads),0,-sin(rads));
        cols[1] = vec3(0,1,0);
        cols[2] = vec3(sin(rads),0,cos(rads));
        return mat3(cols[0], cols[1], cols[2]);
    }

    static mat3 rot_z(double rads){
        vec3 cols[3];
        cols[0] = vec3(cos(rads),sin(rads),0);
        cols[1] = vec3(-sin(rads),cos(rads),0);
        cols[2] = vec3(0,0,1);
        return mat3(cols[0], cols[1], cols[2]);
    }

    static mat3 idt(){
        vec3 cols[3];
        cols[0] = vec3(1,0,0);
        cols[1] = vec3(0,1,0);
        cols[2] = vec3(0,0,1);
        return mat3(cols[0], cols[1], cols[2]);
        
    }

    inline vec3 multiply(const vec3& v) const{
        return e[0]*v.x() + e[1]*v.y() + e[2]*v.z();
    }

    inline mat3 multiply(const mat3& m) const{
        mat3 rslt;
        rslt.e[0] = multiply(m.e[0]);
        rslt.e[1] = multiply(m.e[1]);
        rslt.e[2] = multiply(m.e[2]);
        return rslt;
    }

    void print(){
        std::clog << "COL1: " << e[0] << "\nCOL2: " << e[1] << "\nCOL3: " << e[2] << "\n" << std::endl;
    }

};

#endif 