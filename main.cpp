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
#include "box.h"
#include "constant_medium.h"

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

    cam.render(world, nullptr);
   
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

    cam.render(world, nullptr);
   
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

    cam.render(world, nullptr);
   
}

void quad_scene() {
    hittable_list world;

    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));
    auto materialGlass   = make_shared<dielectric>(1.5);
    
    // Quads
    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));
    world.add(make_shared<sphere>(point3(2,0,0), 2, materialGlass));

    camera cam;

    cam.aspectRatio = 1;
    cam.imgWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxRayBounce = 50;
    cam.vertFOV = 80.0;
    cam.defocusAngle = 0;
    
    auto skybox_tex = make_shared<solid_color_tex>(1.0,1.0,1.0);

    cam.skybox = skybox_tex;
    cam.lookfrom = point3(0,0,9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.render(world, nullptr);
    //cam.initialize();
    //ray r = cam.get_ray(200, 200);
    //hit_record hr;
    //std::clog << "Sending Ray " << r << std::endl;
    //std::clog << "Final Ray color is " << cam.ray_color(r, world, 50) << std::endl; 
}

void simple_light() {
    hittable_list world;

    auto pertext = make_shared<noise_tex>(4);
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<emissive_mat>(color(4,4,4));
        world.add(make_shared<sphere>(point3(0,7,0), 2, difflight));
    world.add(make_shared<quad>(point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), difflight));

    camera cam;

    cam.aspectRatio = 16.0 / 9.0;
    cam.imgWidth = 400;
    cam.samplesPerPixel = 100;
    cam.maxRayBounce = 50;
    cam.vertFOV     = 20;
    cam.defocusAngle = 0;
    
    auto skybox_tex = make_shared<solid_color_tex>(0.0,0.0,0.0);
    cam.skybox = skybox_tex;

    cam.lookfrom = point3(26,3,6);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);


    cam.render(world, nullptr);
}

void cornell_box(){
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<emissive_mat>(color(15, 15, 15));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    auto light_hittable = make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light);
    world.add(light_hittable);
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    auto box1 = make_shared<box>(point3(130, 0, 65), point3(295, 165, 230), white);
    box1->rotate(0,-15,0);
    world.add(box1);
    auto box2 = make_shared<box>(point3(265, 0, 295), point3(430, 330, 460), white);
    box2->rotate(0,18,0);
    world.add(box2);

    camera cam;

    cam.aspectRatio      = 1.0;
    cam.imgWidth       = 600;
    cam.samplesPerPixel = 1000;
    cam.maxRayBounce         = 100;

    auto skybox_tex = make_shared<solid_color_tex>(0.0,0.0,0.0);
    cam.skybox = skybox_tex;

    cam.vertFOV     = 40;

    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(world, light_hittable);
    /*
    cam.initialize();
    ray r = cam.get_ray(300, 300, 8, 8);
    hit_record hr;
    std::clog << "Sending Ray " << r << std::endl;
    std::clog << "Final Ray color is " << cam.ray_color(r, world, 50, light_hittable) << std::endl; 
    */
    
}


void fognell_box(){
    hittable_list world;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<emissive_mat>(color(15, 15, 15));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    auto box1 = make_shared<box>(point3(130, 0, 65), point3(295, 165, 230), white);
    box1->rotate(0,-15,0);
    
    auto box2 = make_shared<box>(point3(265, 0, 295), point3(430, 330, 460), white);
    box2->rotate(0,18,0);
    
    world.add(make_shared<constant_medium>(box1, 0.01, color(0,0,0)));
    world.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    camera cam;

    cam.aspectRatio      = 1.0;
    cam.imgWidth       = 600;
    cam.samplesPerPixel = 200;
    cam.maxRayBounce         = 50;

    auto skybox_tex = make_shared<solid_color_tex>(0.0,0.0,0.0);
    cam.skybox = skybox_tex;

    cam.vertFOV     = 40;

    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(world, nullptr);
    /*cam.initialize();
    ray r = cam.get_ray(300, 515);
    hit_record hr;
    std::clog << "Sending Ray " << r << std::endl;
    std::clog << "Final Ray color is " << cam.ray_color(r, world, 50) << std::endl; */

}


void final_scene(int image_width, int samples_per_pixel, int max_depth) {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = randDouble(1,101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    hittable_list world;

    world.add(make_shared<bvh_node>(boxes1));

    auto light = make_shared<emissive_mat>(color(7, 7, 7));
    world.add(make_shared<quad>(point3(123,554,147), vec3(300,0,0), vec3(0,0,265), light));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30,0,0);
    auto sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(center1, 50, sphere_material));

    world.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(point3(360,150,145), 70, make_shared<dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0,0,0), 5000, make_shared<dielectric>(1.5));
    world.add(make_shared<constant_medium>(boundary, .0001, color(1,1,1)));

    auto emat = make_shared<lambertian>(make_shared<image_tex>("images/earthmap.jpg"));
    world.add(make_shared<sphere>(point3(400,200,400), 100, emat));
    auto pertext = make_shared<noise_tex>(0.2);
    world.add(make_shared<sphere>(point3(220,280,300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0,165) + vec3(-100,270,395), 10, white));
    }
    world.add(make_shared<bvh_node>(boxes2));
    camera cam;

    cam.aspectRatio      = 1.0;
    cam.imgWidth       = image_width;
    cam.samplesPerPixel = samples_per_pixel;
    cam.maxRayBounce         = max_depth;
    
    auto skybox = make_shared<solid_color_tex>(color(0,0,0));
    cam.skybox        = skybox;

    cam.vertFOV     = 40;
    cam.lookfrom = point3(478, 278, -600);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocusAngle = 0;

    cam.render(world, nullptr);
}



int main(){
    srand(time(NULL));
    prfl::create_profile(WHOLE_EXEC, "runtime");
    prfl::start_profiling_segment(WHOLE_EXEC);
    
    cornell_box();
    //final_scene(800, 10000, 40);
    prfl::end_profiling_segment(WHOLE_EXEC);
    prfl::print_full_profiler_info();
}