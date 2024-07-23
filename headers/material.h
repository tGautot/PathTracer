#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"
#include "hittable.h"


class material {
public:

    virtual ~material() = default;

    virtual bool scatter(const ray& rayIn, hit_record& hr, color& attenuation, ray& rayOut) const{
        return false;
    }

};


class lambertian : public material {
private:
    color albedo;

public:
    lambertian(const color& albedo): albedo(albedo){}

    bool scatter(const ray& rayIn, hit_record& hr, color& attenuation, ray& rayOut) const override{
        vec3 scatterDir = hr.normal + vec3::random_on_unit_sphere();

        if(scatterDir.near_zero())
            scatterDir = hr.normal;

        rayOut = ray(hr.p, scatterDir);
        attenuation = albedo;
        return true;
    }



};


class metal : public material {
private:
    color albedo;
    double fuzzFactor;

public:
    metal(const color& albedo, double fuzz): albedo(albedo), fuzzFactor(fuzz) {}

    bool scatter(const ray& rayIn, hit_record& hr, color& attenuation, ray& rayOut) const override{
        vec3 scatterDir = rayIn.direction().reflect(hr.normal).normalized();
        scatterDir += fuzzFactor * vec3::random_in_hemisphere(scatterDir);

     

        rayOut = ray(hr.p, scatterDir);
        attenuation = albedo;
        return dot(scatterDir, hr.normal) > 0;
    }

};


class dielectric: public material {
private:
    double refractiveIndex;
    
public:
    dielectric(double ri): refractiveIndex(ri) {}

    bool scatter(const ray& rayIn, hit_record& hr, color& attenuation, ray& rayOut) const override{
        attenuation = color(1,1,1);

        double ri = hr.front_face ? 1/refractiveIndex : refractiveIndex;
        vec3 normalizedInDir = rayIn.direction().normalized();

        double cosTheta = fmin(dot(-normalizedInDir, hr.normal), 1);
        double sinTheta = sqrt(1.0-cosTheta*cosTheta);
        
        bool shouldReflect = ri*sinTheta > 1.0 || reflectance(cosTheta, ri) > randDouble();

        vec3 scatterDir;
        if(shouldReflect){
            scatterDir = reflect(rayIn.direction(), hr.normal);
        } else {
            scatterDir = refract(normalizedInDir, hr.normal, ri);
        }


        rayOut = ray(hr.p, scatterDir);

        return true;
    }    

private:
    static double reflectance(double cosine, double ri) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ri) / (1 + ri);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};


#endif