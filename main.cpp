#include "common.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "animated.h"
#include "bvh.h"
#include "time_profiler.h"

void complex_scene(){
    hittable_list world;

    auto checker = make_shared<checker_tex>(color(0.78, 0.41, 1.0), color(0.75, 0.89, 0.74));
    checker->set_size(0.32);
    auto ground_material = make_shared<lambertian>(checker);
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

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));



    world = hittable_list(make_shared<bvh_node>(world));


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


void center_sphere_move(double t, sphere& _base){
    _base.center[2] += (t-0.5)*2;
}

void simple_scene() {
    hittable_list world;
    
    auto materialGround = make_shared<lambertian>(color(0.8,0.8,0.0));
    auto materialCenter = make_shared<lambertian>(color(0.1,0.2,0.5));
    auto materialLeft   = make_shared<dielectric>(1.5);
    auto materialRight  = make_shared<metal>(color(0.8,0.6,0.2), 0.9);
    auto materialBubble = make_shared<dielectric>(1.0/1.5);

    auto center_sphere = make_shared<sphere>(sphere(point3(0,0,1.2), 0.5, materialCenter));

    //world.add(make_shared<sphere>(point3(0,-100.5,-1), 100, materialGround));
    world.add(center_sphere);
    world.add(make_shared<sphere>(point3(-1.1,0,1.0), 0.5, materialLeft));
    world.add(make_shared<sphere>(point3(-1.1,0,1.0), 0.4, materialBubble));
    world.add(make_shared<sphere>(point3(1.1,0,1.0), 0.5, materialRight));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;
    cam.aspectRatio = 16.0/9.0;
    cam.imgWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxRayBounce = 20;
    cam.vertFOV = 40.0;
    cam.focusDist = 3.4;
    cam.defocusAngle = 1;

    cam.lookfrom = point3(-2,2,-1);
    cam.lookat   = point3(0,0,1.2);
    cam.vup      = vec3(0,1,0);

    cam.render(world);
   
    std::clog << "\n\rDone done.                                     \n";

}

void extra_simple_scene(){
    hittable_list world;
    
    auto mat = make_shared<lambertian>(color(0.0,0.8,0.8));
    auto singleSphere = make_shared<sphere>(sphere(point3(0,0,1.2), 0.5, mat));
    world.add(singleSphere);
    world = hittable_list(make_shared<bvh_node>(world));
    camera cam;
    cam.aspectRatio = 16.0/9.0;
    cam.imgWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxRayBounce = 20;
    cam.vertFOV = 40.0;
    cam.focusDist = 3.4;
    cam.defocusAngle = 1;

    cam.lookfrom = point3(-2,2,-1);
    cam.lookat   = point3(0,0,1.2);
    cam.vup      = vec3(0,1,0);

    cam.render(world);
   
}

int main(){
    prfl::create_profile(WHOLE_EXEC, "runtime");
    prfl::start_profiling_segment(WHOLE_EXEC);
    complex_scene();
    prfl::end_profiling_segment(WHOLE_EXEC);
    prfl::print_full_profiler_info();
}