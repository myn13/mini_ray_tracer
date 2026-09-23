#include "src/geometry/cube.h"
#include "src/geometry/sphere.h"
#include "src/core/ray.h"
#include "src/core/color.h"
#include "src/camera/camera.h"
#include "src/materials/lambertian.h"
#include "src/materials/metal.h"
#include "src/materials/dielectric.h"
#include "src/geometry/hittable_list.h"
#include "src/bvh_tree/bvh.h"
#include "src/geometry/quad.h"

int main() {
    color maroon = decimal_to_color(128, 0, 0);
    color metallic = decimal_to_color(204,204,204);
    color turquoise = decimal_to_color(64, 224, 208);
    color lime = decimal_to_color(50, 205, 50);
    color sky = decimal_to_color(135, 206, 235);
    color sienna = decimal_to_color(160, 82, 45);
    color pastel_pink = decimal_to_color(248, 200, 220);
    color purple = decimal_to_color(147, 112, 219);
    color off_white = decimal_to_color(250, 249, 246);

    // Materials
    auto red_lambertian = std::make_shared<lambertian>(maroon);
    auto purple_lambertian = std::make_shared<lambertian>(purple);
    auto lime_lambertian = std::make_shared<lambertian>(lime);
    auto turquoise_lambertian = std::make_shared<lambertian>(turquoise);
    auto offwhite_lambertian = std::make_shared<lambertian>(off_white);
    auto metal_mat = std::make_shared<metal>(metallic, 0.01);
    auto glass_mat = std::make_shared<dielectric>(off_white, 1.5);
    auto turquoise_box_mat = std::make_shared<lambertian>(turquoise);
    auto lime_box_mat = std::make_shared<lambertian>(lime);

    // Camera
    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.sample_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;
    std::vector<int> values(10000);
    auto start = std::chrono::high_resolution_clock::now();

    // World
    hittable_list world;
    
    // Cornell Box
    world.add(std::make_shared<quad>(point3(-2,-2, 0), vec3(0, 0, 4), vec3(0, 4, 0), red_lambertian));
    world.add(std::make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), lime_lambertian)); // back
    world.add(std::make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), purple_lambertian));
    world.add(std::make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), offwhite_lambertian));
    world.add(std::make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), turquoise_lambertian));

    // // object at the center
    world.add(std::make_shared<sphere>(point3(-3,-2, 5), 0.5, red_lambertian));
    // world.add(std::make_shared<sphere>(point3(-15, -41.875, -20), 13.125, lime_box_mat));
    // world.add(std::make_shared<cube>(point3(20, -49, -20), 12, metal_mat));
    // world.add(std::make_shared<cube>(point3(24, -37, -20), 12, glass_mat));

    // Build world with bvh
    // std::cout << "World has" << world.objects.size() << std::endl;
    auto bvh_world = std::make_shared<bvh_node>(world);

    // cam.render(world);
    cam.render(*bvh_world);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Done rendering after " << duration.count() << " microseconds" << std::endl;
    return 0;
}