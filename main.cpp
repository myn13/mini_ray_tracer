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
    auto pink_wall = std::make_shared<lambertian>(pastel_pink);
    auto purple_wall =  std::make_shared<lambertian>(purple);
    auto offwhite_wall = std::make_shared<lambertian>(off_white);
    auto ground = std::make_shared<lambertian>(sienna);
    auto red_sphere = std::make_shared<lambertian>(maroon);
    auto metal_mat = std::make_shared<metal>(metallic, 0.01);
    auto glass_mat = std::make_shared<dielectric>(off_white, 1.5);
    auto turquoise_box_mat = std::make_shared<lambertian>(turquoise);
    auto lime_box_mat = std::make_shared<lambertian>(lime);
    auto lower_teal   = std::make_shared<lambertian>(color(0.2, 0.8, 0.8));
    // Camera
    camera cam;
    std::vector<int> values(10000);
    auto start = std::chrono::high_resolution_clock::now();

    // World
    hittable_list world;
    
    // The walls

    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0, -20), vec3(0, 20, 0), offwhite_wall));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(20, 0, 0), vec3(0, 20, 0), pink_wall));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(20, 0, 4), vec3(0, 20, 0), purple_wall));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(20, 0, 0), vec3(0, 0, 20), ground));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(20, 0, 0), vec3(0, 0, -20), lower_teal));

    // // object at the center
    // world.add(std::make_shared<sphere>(point3(0, 0, 0), 5, turquoise_box_mat));
    // world.add(std::make_shared<sphere>(point3(-15, -41.875, -20), 13.125, lime_box_mat));
    // world.add(std::make_shared<cube>(point3(20, -49, -20), 12, metal_mat));
    // world.add(std::make_shared<cube>(point3(24, -37, -20), 12, glass_mat));

    // Build world with bvh
    // std::cout << "World has" << world.objects.size() << std::endl;
    auto bvh_world = std::make_shared<bvh_node>(world);

    // cam.render(world);
    cam.render(*bvh_world);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Done rendering after " << duration.count() << " microseconds" << std::endl;
    return 0;
}