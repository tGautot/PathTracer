#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "common.h"

class camera{
public:

    double aspectRatio = 1.0;
    int imgWidth = 400;
    int samplesPerPixel = 10;
    int maxRayBounce = 10;

    double vertFOV = 90.0;
    double defocusAngle = 0.0;
    double focusDist = 10;

    point3 lookfrom = point3(0,0,0);
    point3 lookat = point3(0,0,1);
    vec3 vup = vec3(0,1,0);

    camera(){

    }


    void render(const hittable& world){
        initialize();

        std::cout << "P3\n" << imgWidth << ' ' << imgHeight << "\n255\n";
        for(int i = 0; i < imgHeight; i++){
            std::clog << "\rScan lines " << (i+1) << "/" << imgHeight << " (" << int(100*(i+1)/imgHeight) << "%)       " << std::flush;
            for(int j = 0; j < imgWidth; j++){
                color totCol = color(0,0,0);
                for(int k = 0; k < samplesPerPixel; k++){
                    ray pixelRay = get_ray(i, j);
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

        vec3 vpUpperLeft = cameraPos - focusDist*w - vp_U/2 - vp_V/2;
        pixel00Loc = vpUpperLeft + 0.5*(pixelDeltaU + pixelDeltaV);
    
        double defocusRadius = focusDist * tan(degsToRads(defocusAngle/2));
        defocusDiskU = u * defocusRadius;
        defocusDiskV = v * defocusRadius;
    }

    ray get_ray(int pix_i, int pix_j) const{
        point3 pixLoc = pixel00Loc + pix_i * pixelDeltaV + pix_j * pixelDeltaU;
        vec3 pixOffset = (randDouble()-0.5) * pixelDeltaU + (randDouble()-0.5) * pixelDeltaV;
        point3 rayOrigin = cameraPos;
        if(defocusAngle > EPSILON)
            rayOrigin = sample_defocus_disk();
        ray r = ray(rayOrigin, pixLoc + pixOffset - rayOrigin, randDouble());
        return r;
    }

    point3 sample_defocus_disk() const{
        vec3 v = vec3::random_in_unit_circle();
        return cameraPos +v[0]*defocusDiskU + v[1]*defocusDiskV; 
    }

    color ray_color(const ray& r, const hittable& world, int bouncesLeft){
        hit_record hr;
        if(bouncesLeft < 0)
            return color(0,0,0);
        if (world.hit(r, interval(0.001, infinity), hr)){
            ray scattered;
            color attenuation;
            if(hr.mat->scatter(r, hr, attenuation, scattered))
                return attenuation*ray_color(scattered, world, bouncesLeft-1);
            return color(0,0,0);
        }
            

        vec3 n = vec3(r.direction()).normalized();
        double a = (n.y()+1.0)*0.5;
        return a*color(1,1,1) + (1-a)*color(0.5,0.7,1);
    }

};


#endif