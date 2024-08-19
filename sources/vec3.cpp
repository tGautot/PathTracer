#include "common.h"


vec3 vec3::random_on_unit_sphere(){
    // let S be the unit sphere: xx + yy + zz = 1,
    // Let C be the circle defined by yy + zz = 1 - vxvx
    // We can then simply sample a random point on this circle
    /*double radius = 1;
    double vx = randDouble(-radius,radius);
    radius = 1-vx*vx;
    double vy = randDouble(-radius,radius);
    double vz = sqrt(radius*radius- vy*vy);
    if(randDouble() < 0.5)
        vz = -vz;
    return vec3(vx,vy,vz);*/
    return random_in_unit_sphere().normalized();
}

vec3 vec3::random_in_unit_sphere(){
    //double radius = randDouble();
    //return vec3::random_on_unit_sphere() * radius;

    while(true){
        vec3 rdm = vec3::random(-1,1);
        if(rdm.length() > 1) continue;
        return rdm;
    }
}

vec3 vec3::random_on_unit_circle(){
    /*double radius = 1;
    double vx = randDouble(-radius,radius);
    double vy = sqrt(radius*radius- vx*vx);
    if(randDouble() < 0.5)
        vy = -vy;
    return vec3(vx,vy,0);*/
    return random_in_unit_circle().normalized();
}

vec3 vec3::random_in_unit_circle(){
    //double radius = randDouble();
    //return vec3::random_on_unit_circle() * radius;

    while(true){
        vec3 rdm = vec3::random(-1,1);
        rdm.e[2] = 0;
        if(rdm.length() > 1) continue;
        return rdm;
    }
}

vec3 vec3::random_in_hemisphere(const vec3& normal){
    vec3 v = random_in_unit_sphere();
    if(dot(v, normal) < 0){
        v = -v;
    }

    return v;
}

vec3 vec3::random_on_hemisphere(const vec3& normal){
    vec3 v = random_on_unit_sphere();
    if(dot(v, normal) < 0){
        v = -v;
    }

    return v;
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

void write_vector(std::ostream& out, const vec3& v){
    out << v.x() << ' ' << v.y() << ' ' << v.z() << '\n';
}