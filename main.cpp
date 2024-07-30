#include "common.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "animated.h"
#include "bvh.h"
#include "time_profiler.h"
#include "shape2d.h"

void complex_scene(){
    hittable_list world;

    auto earth_tex = make_shared<image_tex>("images/earthmap.jpg");
    auto earth_mat = make_shared<lambertian>(earth_tex);

    auto noiseTex = make_shared<noise_tex>(4);
    auto noiseMat = make_shared<lambertian>(noiseTex);
    

    auto checker = make_shared<checker_tex>(color(0.78, 0.41, 1.0), color(0.75, 0.89, 0.74));
    checker->set_size(0.32);
    auto ground_material = make_shared<lambertian>(checker);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, noiseMat));

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

    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, earth_mat));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world.add(make_shared<sphere>(point3(8,1,1), 1.0, noiseMat));

    world = hittable_list(make_shared<bvh_node>(world));


    camera cam;
    cam.aspectRatio = 16.0/9.0;
    cam.imgWidth = 400;
    cam.samplesPerPixel = 200;
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

void quad_scene() {
    hittable_list world;

    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    camera cam;

    cam.aspectRatio = 1;
    cam.imgWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxRayBounce = 50;
    cam.vertFOV = 80.0;
    cam.defocusAngle = 0;

    cam.lookfrom = point3(0,0,9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.render(world);
    /*cam.initialize();
    ray r = cam.get_ray(200, 200);
    hit_record hr;
    std::cout << "Sending Ray " << r << std::endl;
    world.hit(r, interval::universe, hr);*/
}

int main(){
    prfl::create_profile(WHOLE_EXEC, "runtime");
    prfl::start_profiling_segment(WHOLE_EXEC);
    quad_scene();
    prfl::end_profiling_segment(WHOLE_EXEC);
    prfl::print_full_profiler_info();
}