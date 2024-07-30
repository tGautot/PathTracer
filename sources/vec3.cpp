#include "common.h"


vec3 vec3::random_on_unit_sphere(){
    double radius = 1;
    double vx = randDouble(-radius,radius);
    // let S be the unit sphere: xx + yy + zz = 1,
    // Let C be the circle defined by yy + zz = 1 - vxvx
    // We can then simply sample a random point on this circle
    radius = 1-vx*vx;
    double vy = randDouble(-radius,radius);
    double vz = sqrt(radius*radius- vy*vy);
    if(randDouble() < 0.5)
        vz = -vz;
    return vec3(vx,vy,vz);
}

vec3 vec3::random_in_unit_sphere(){
    double radius = randDouble();
    return vec3::random_on_unit_sphere() * radius;
}

vec3 vec3::random_on_unit_circle(){
    double radius = 1;
    double vx = randDouble(-radius,radius);
    double vy = sqrt(radius*radius- vx*vx);
    if(randDouble() < 0.5)
        vy = -vy;
    return vec3(vx,vy,0);
}

vec3 vec3::random_in_unit_circle(){
    double radius = randDouble();
    return vec3::random_on_unit_circle() * radius;
}

vec3 vec3::random_in_hemisphere(const vec3& normal){
    vec3 v = random();
    if(dot(v, normal) < 0){
        v = -v;
    }

    return v.normalized();
}

vec3 vec3::project(const vec3& onto) const{
    return onto * (dot(*this, onto) / onto.length_squared());
}

vec3 vec3::reflect(const vec3& nrml) const {
    return *this - 2.0*project(nrml);
}

std::ostream& operator<<(std::ostream& out, const vec3& r){
    return out << "["<< r.x() << "," << r.y() << "," << r.z() << "]";   
}