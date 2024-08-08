#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "common.h"
#include "texture.h"
#include "pdf.h"

#include <memory>

class camera{
public:

    double aspectRatio = 1.0;
    int imgWidth = 400;
    int samplesPerPixel = 10;
    int maxRayBounce = 10;

    double vertFOV = 90.0;
    double defocusAngle = 0.0;
    double focusDist = 10;

    double strat_factor_u = -1, strat_factor_v = -1;
    int strat_count_u, strat_count_v;


    shared_ptr<texture> skybox;

    point3 lookfrom = point3(0,0,0);
    point3 lookat = point3(0,0,1);
    vec3 vup = vec3(0,1,0);

    camera(){

    }


    void render(const hittable& world){
        initialize();
        std::clog << "Each pixel will be stratified into " << strat_count_u << "x" << strat_count_v <<std::endl;
        std::cout << "P3\n" << imgWidth << ' ' << imgHeight << "\n255\n";
        for(int i = 0; i < imgHeight; i++){
            std::clog << "\rScan lines " << (i+1) << "/" << imgHeight << " (" << int(100*(i+1)/imgHeight) << "%)       " << std::flush;
            for(int j = 0; j < imgWidth; j++){
                color totCol = color(0,0,0);
                #pragma omp parallel for 
                for(int k = 0; k < strat_count_u*strat_count_v; k++){
                    int ku = k % strat_count_u;
                    int kv = k / strat_count_u;
                    ray pixelRay = get_ray(i, j, ku, kv);
                    totCol += ray_color(pixelRay, world, maxRayBounce);
                }
                write_color(std::cout, totCol/samplesPerPixel);
            }
            std::cout << '\n';
        }
        std::cout << std::endl;
    }

private:
    int imgHeight;
    point3 cameraPos, pixel00Loc;
    vec3 pixelDeltaU, pixelDeltaV;
    vec3 u,v,w;
    vec3 defocusDiskU, defocusDiskV;
    vec3 vpUpperLeft;

    vec3 stratified_pix_du, stratified_pix_dv;


public:
    void initialize(){

        imgHeight = int(double(imgWidth)/aspectRatio);
        imgHeight = (imgHeight < 1) ? 1 : imgHeight;

        double theta = degsToRads(vertFOV);
        double h = tan(theta/2.0);
        double vpHeight = 2.0 * h * focusDist;
        double vpWidth = vpHeight * (double(imgWidth) / double(imgHeight));
        
        
        cameraPos = lookfrom;

        w = (lookfrom - lookat).normalized();
        u = cross(vup, w).normalized();
        v = cross(w, u); 

        vec3 vp_U = vpWidth  * u;
        vec3 vp_V = vpHeight * -v;

        pixelDeltaU = vp_U / imgWidth;
        pixelDeltaV = vp_V / imgHeight;

        vpUpperLeft = cameraPos - focusDist*w - vp_U/2 - vp_V/2;
        pixel00Loc = vpUpperLeft + 0.5*(pixelDeltaU + pixelDeltaV);

        if(strat_factor_u < 0 || strat_factor_v < 0){
            strat_factor_u = strat_factor_v = 1.0/sqrt(samplesPerPixel);
        }
        strat_count_u = int(1.0/strat_factor_u);
        strat_count_v = int(1.0/strat_factor_v);
        stratified_pix_du = pixelDeltaU * strat_factor_u;
        stratified_pix_dv = pixelDeltaV * strat_factor_v;

        double defocusRadius = focusDist * tan(degsToRads(defocusAngle/2));
        defocusDiskU = u * defocusRadius;
        defocusDiskV = v * defocusRadius;
    }

    ray get_ray(int pix_i, int pix_j, int st_i, int st_j) const{
        point3 pixTopLeftCorner = vpUpperLeft + pix_i * pixelDeltaV + pix_j * pixelDeltaU;
        vec3 pixOffset = sample_stratified_rect(stratified_pix_du, stratified_pix_dv, st_i, st_j); // stratification
        point3 rayOrigin = cameraPos;
        if(defocusAngle > EPSILON)
            rayOrigin = sample_defocus_disk();
        ray r = ray(rayOrigin, pixTopLeftCorner + pixOffset - rayOrigin, randDouble());
        return r;
    }

    vec3 sample_stratified_rect(const vec3& du, const vec3& dv, int idu, int idv) const {
        return (idu + randDouble())*du + (idv + randDouble())*dv;
    }

    point3 sample_defocus_disk() const{
        vec3 v = vec3::random_in_unit_circle();
        return cameraPos +v[0]*defocusDiskU + v[1]*defocusDiskV; 
    }

    color ray_color(const ray& r, const hittable& world, int bouncesLeft){
        hit_record hr;
        if(bouncesLeft < 0){
            //std::clog << "RAY RAN OUT OF BOUNCES" << std::endl;
            return color(0,0,0);
        }
        if (world.hit(r, interval(0.001, infinity), hr)){
            ray scattered;
            color attenuation;
            color emitted = hr.mat->emitted(hr.u, hr.v, hr.p);
            double pdfval;
            if(!hr.mat->scatter(r, hr, attenuation, scattered, pdfval)){
                //std::clog << "RAY DIDNT SCATTER" << std::endl;
                return emitted;
            }

            cosine_hemisphere_pdf scatter_gen(hr.normal);
            scattered = ray(hr.p, scatter_gen.generate(), hr.t);
            pdfval = scatter_gen.val(scattered.direction());

            //std::clog << "hit color is " << attenuation << std::endl;
            double mat_scatter_pdf = hr.mat->scatter_pdf(r, hr, scattered);

            color scatter_col = mat_scatter_pdf*attenuation*ray_color(scattered, world, bouncesLeft-1)/pdfval;
            return emitted + scatter_col;
        }
            
        //std::clog << "RAY HIT SKYBOX" << std::endl;
        vec3 n = r.direction().normalized();
        double u, v;
        sphere::get_sphere_uv(n, u, v);
        return skybox->value(u, v, n);
    }

};


#endif