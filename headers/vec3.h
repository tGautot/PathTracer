#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include "utils.h"
#include "cuda.h"

class vec3 {
public:
    cu_double e[3];

    CUDA_BOTH vec3(): e{0,0,0} {}
    CUDA_BOTH vec3(const vec3& cv):e{cv.e[0], cv.e[1], cv.e[2]} {}
    CUDA_BOTH vec3(cu_double e1, cu_double e2, cu_double e3): e{e1,e2,e3} {}


    CUDA_BOTH cu_double x() const {return e[0];}
    CUDA_BOTH cu_double y() const {return e[1];}
    CUDA_BOTH cu_double z() const {return e[2];}

    void set(const vec3& v){
        e[0] = v.e[0];
        e[1] = v.e[1];
        e[2] = v.e[2];
    }

    static vec3 random(){
        return vec3(randDouble(-1,1), randDouble(-1,1), randDouble(-1,1));
    }

    static vec3 random(double min, double max){
        return vec3(randDouble(min,max), randDouble(min,max), randDouble(min,max));
    }

    static vec3 random_on_unit_sphere(); // .cpp

    static vec3 random_in_unit_sphere(); // .cpp

    static vec3 random_on_unit_circle(); // .cpp

    static vec3 random_in_unit_circle(); // .cpp

    static vec3 random_in_hemisphere(const vec3& normal); // .cpp

    static vec3 random_on_hemisphere(const vec3& normal); // .cpp

    vec3 project(const vec3& onto) const; // .cpp

    vec3 reflect(const vec3& nrml) const; // .cpp

    CUDA_BOTH bool near_zero() const{
        return length_squared() < EPSILON;
    }

    std::ostream& to_stream(std::ostream& out) const{
        return out << "(" << e[0] << ", " << e[1] << ", " << e[2] << ") ";
    }

    CUDA_BOTH vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
    CUDA_BOTH cu_double operator[](int i) const {return e[i];}
    CUDA_BOTH cu_double& operator[](int i) {return e[i];}

    CUDA_BOTH vec3& operator+=(const vec3& v){
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    CUDA_BOTH vec3& operator*=(cu_double t){
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    CUDA_BOTH vec3& operator/=(cu_double t){
        e[0] /= t;
        e[1] /= t;
        e[2] /= t;
        return *this;
    }

    CUDA_BOTH cu_double length() const {
        return std::sqrt(length_squared());
    }

    CUDA_BOTH cu_double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    CUDA_BOTH vec3 normalized() const{
        cu_double l = length();
        return vec3(e[0]/l, e[1]/l, e[2]/l);
    }

};



CUDA_BOTH inline cu_double dot(const vec3& a, const vec3& b){
    return  a.e[0] * b.e[0] +
            a.e[1] * b.e[1] +
            a.e[2] * b.e[2];
}

CUDA_BOTH inline vec3 cross(const vec3& a, const vec3& b) {
    return vec3(a.e[1] * b.e[2] - a.e[2] * b.e[1],
                a.e[2] * b.e[0] - a.e[0] * b.e[2],
                a.e[0] * b.e[1] - a.e[1] * b.e[0]);
}

CUDA_BOTH inline vec3 operator+(const vec3& a, const vec3& b){
    return vec3(a.e[0]+b.e[0], a.e[1]+b.e[1], a.e[2]+b.e[2]);
}

CUDA_BOTH inline vec3 operator-(const vec3& a, const vec3& b){
    return vec3(a.e[0]-b.e[0], a.e[1]-b.e[1], a.e[2]-b.e[2]);
}

CUDA_BOTH inline vec3 operator*(const vec3& a, const vec3& b){
    return vec3(a.e[0]*b.e[0], a.e[1]*b.e[1], a.e[2]*b.e[2]);
}

CUDA_BOTH inline vec3 operator*(const vec3& v, double t){
    return vec3(v.e[0]*t, v.e[1]*t, v.e[2]*t);
}

CUDA_BOTH inline vec3 operator*(double t, const vec3& v){
    return v*t;
}

CUDA_BOTH inline vec3 operator/(const vec3& v, double t){
    return v*(1/t);
}

CUDA_BOTH inline bool operator==(const vec3& v1, const vec3& v2){
    return std::fabs(v1.x() - v2.x()) < EPSILON && std::fabs(v1.y() - v2.y()) < EPSILON && std::fabs(v1.z() - v2.z()) < EPSILON;
}

CUDA_BOTH inline bool operator!=(const vec3& v1, const vec3& v2){
    return !(v1 == v2);
}

CUDA_BOTH inline bool operator>(const vec3& v1, double d){
    return v1.x() > d || v1.y() > d || v1.z() > d;
}

CUDA_BOTH inline vec3 project(const vec3& v, const vec3& onto){
    return onto * (dot(v, onto) / onto.length_squared());
}

CUDA_BOTH inline vec3 reflect(const vec3& v, const vec3& nrml) {
    return v - 2.0*project(v,nrml);
}

CUDA_BOTH inline vec3 refract(const vec3& in, const vec3& nrml, cu_double rri){
    cu_double cosTheta = fmin(dot(-in, nrml), 1.0);
    vec3 outPerp = rri * (in + cosTheta*nrml);
    vec3 outParl = -sqrt(fabs(1.0-outPerp.length_squared()))*nrml;
    return outParl + outPerp;
}

inline std::ostream& operator<<(std::ostream& o, const vec3 v){
    return o << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

void write_vector(std::ostream& out, const vec3& v);

using point3 = vec3;


#endif