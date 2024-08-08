#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"
#include "hittable.h"
#include "texture.h"


class material {
public:

    virtual ~material() = default;

    virtual color emitted(double u, double v, const point3& p) const {
        return color(0,0,0);
    }

    virtual bool scatter(const ray& rayIn, hit_record& hr, color& attenuation, ray& rayOut, double& chosen_dir_pdf) const{
        return false;
    }

    virtual double scatter_pdf(const ray& ray_in, const hit_record& hr, const ray& ray_out) const {
        return 0.0;
    }
};


class lambertian : public material {
private:
    shared_ptr<texture> albedo;

public:
    lambertian(color  albedo): albedo(make_shared<solid_color_tex>(albedo)){}
    lambertian(shared_ptr<texture> tex): albedo(tex){}

    bool scatter(const ray& rayIn, hit_record& hr, color& attenuation, ray& rayOut, double& chosen_dir_pdf) const override{
        vec3 scatterDir = /*vec3::random_on_hemisphere(hr.normal);*/ hr.normal + vec3::random_on_unit_sphere();

        if(scatterDir.near_zero())
            scatterDir = hr.normal;

        rayOut = ray(hr.p, scatterDir, rayIn.time());
        attenuation = albedo->value(hr.u,hr.v,hr.p);

        chosen_dir_pdf = scatter_pdf(rayIn, hr, rayOut);

        return true;
    }

    double scatter_pdf(const ray& ray_in, const hit_record& hr, const ray& ray_out) const {
        double cos_theta = dot(hr.normal, ray_out.direction().normalized());
        return cos_theta < 0.0 ? 0.0 : cos_theta/PI;
    }



};


class metal : public material {
private:
    color albedo;
    double fuzzFactor;

public:
    metal(const color& albedo, double fuzz): albedo(albedo), fuzzFactor(fuzz) {}

    bool scatter(const ray& rayIn, hit_record& hr, color& attenuation, ray& rayOut, double& chosen_dir_pdf) const override{
        vec3 scatterDir = rayIn.direction().reflect(hr.normal).normalized();
        scatterDir += fuzzFactor * vec3::random_in_hemisphere(scatterDir);


        rayOut = ray(hr.p, scatterDir, rayIn.time());
        attenuation = albedo;
        chosen_dir_pdf = scatter_pdf(rayIn, hr, rayOut);
        return dot(scatterDir, hr.normal) > 0;
    }

    double scatter_pdf(const ray& ray_in, const hit_record& hr, const ray& ray_out) const {
        vec3 perfect_reflect = ray_in.direction().reflect(hr.normal).normalized();
        vec3 scatter_dir = ray_out.direction().normalized();
        double cos_theta = dot(perfect_reflect, scatter_dir);
        return cos_theta < 0.0 ? 0.0 : cos_theta/(fuzzFactor*PI);
        //double cos_theta = dot(hr.normal, ray_out.direction().normalized());
        //return cos_theta < 0.0 ? 0.0 : cos_theta/PI;
    }



};


class dielectric: public material {
private:
    double refractiveIndex;
    
public:
    dielectric(double ri): refractiveIndex(ri) {}

    bool scatter(const ray& rayIn, hit_record& hr, color& attenuation, ray& rayOut, double& chosen_dir_pdf) const override{
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


        rayOut = ray(hr.p, scatterDir, rayIn.time());
        chosen_dir_pdf = scatter_pdf(rayIn, hr, rayOut);

        return true;
    }    

    double scatter_pdf(const ray& ray_in, const hit_record& hr, const ray& ray_out) const {
        double ri = hr.front_face ? 1/refractiveIndex : refractiveIndex;
        vec3 normalizedInDir = ray_in.direction().normalized();

        double cosTheta = fmin(dot(-normalizedInDir, hr.normal), 1);
        double sinTheta = sqrt(1.0-cosTheta*cosTheta);
        if(ri*sinTheta > 1.0) return 1.0;
        double reflect_prob = reflectance(cosTheta, ri);
        vec3 reflected = reflect(ray_in.direction(), hr.normal);
        return reflected == ray_out.direction() ? reflect_prob : 1-reflect_prob;
    }

private:
    static double reflectance(double cosine, double ri) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ri) / (1 + ri);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }


};

class emissive_mat : public material {
private:
    shared_ptr<texture> tex;
    double intensity;
public:

    emissive_mat(shared_ptr<texture> tex, double intensity=1.0): tex(tex), intensity(intensity) {}
    emissive_mat(const color& col, double intensity=1.0): tex(make_shared<solid_color_tex>(col)), intensity(intensity) {}

    color emitted(double u, double v, const point3& p) const override{
        return tex->value(u,v,p)*intensity;
    }



};


class isotropic : public material {
  public:
    isotropic(const color& albedo) : tex(make_shared<solid_color_tex>(albedo)) {}
    isotropic(shared_ptr<texture> tex) : tex(tex) {}

     bool scatter(const ray& rayIn, hit_record& hr, color& attenuation, ray& rayOut, double& chosen_dir_pdf) const override {
        rayOut = ray(hr.p, vec3::random_on_unit_sphere(), rayIn.time());
        attenuation = tex->value(hr.u, hr.v, hr.p);
        chosen_dir_pdf = scatter_pdf(rayIn, hr, rayOut);
        return true;
    }

    double scatter_pdf(const ray& ray_in, const hit_record& hr, const ray& ray_out) const {
        return 1.0 / (4*PI);
    }

  private:
    shared_ptr<texture> tex;
};

#endif