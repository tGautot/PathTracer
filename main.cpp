#include "common.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"





int main(){
    
    /*auto materialGround = make_shared<lambertian>(color(0.8,0.8,0.0));
    auto materialCenter = make_shared<lambertian>(color(0.1,0.2,0.5));
    auto materialLeft   = make_shared<dielectric>(1.5);
    auto materialRight  = make_shared<metal>(color(0.8,0.6,0.2), 0.9);
    auto materialBubble = make_shared<dielectric>(1.0/1.5);

    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100, materialGround));
    world.add(make_shared<sphere>(point3(0,0,1.2), 0.5, materialCenter));
    world.add(make_shared<sphere>(point3(-1,0,1.0), 0.5, materialLeft));
    world.add(make_shared<sphere>(point3(-1,0,1.0), 0.4, materialBubble));
    world.add(make_shared<sphere>(point3(1,0,1.0), 0.5, materialRight));*/

    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randDouble();
            point3 center(a + 0.9*randDouble(), 0.2, b + 0.9*randDouble());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = randDouble(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }


    camera cam;
    cam.aspectRatio = 16.0/9.0;
    cam.imgWidth = 1920;
    cam.samplesPerPixel = 500;
    cam.maxRayBounce = 50;
    cam.vertFOV = 40.0;
    cam.focusDist = 10.0;
    cam.defocusAngle = 0.6;

    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,1.2);
    cam.vup      = vec3(0,1,0);

    cam.render(world);
   
    std::clog << "\n\rDone done.                                     \n";
}