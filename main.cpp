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
#include "src/materials/diffuse.h"

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
    color bright_white(4, 3, 3);

    // Materials
    auto red_lambertian = std::make_shared<lambertian>(maroon);
    auto purple_lambertian = std::make_shared<lambertian>(purple);
    auto lime_lambertian = std::make_shared<lambertian>(lime);
    auto pink_lambertian = std::make_shared<lambertian>(pastel_pink);
    auto turquoise_lambertian = std::make_shared<lambertian>(turquoise);
    auto offwhite_lambertian = std::make_shared<lambertian>(off_white);
    auto metal_mat = std::make_shared<metal>(metallic, 0.01);
    auto glass_mat = std::make_shared<dielectric>(off_white, 1.5);
    auto turquoise_box_mat = std::make_shared<lambertian>(turquoise);
    auto lime_box_mat = std::make_shared<lambertian>(lime);
    auto light = std::make_shared<diffuse_light>(bright_white);

    // Camera
    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.sample_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,6.25);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);
    cam.background = color(0.7, 0.8, 1.0);

    cam.defocus_angle = 0;
    std::vector<int> values(10000);
    auto start = std::chrono::high_resolution_clock::now();

    // World
    hittable_list world;
    
    // Cornell Box
    world.add(std::make_shared<quad>(point3(-2,-2, 0), vec3(0, 0, 4), vec3(0, 4, 0), red_lambertian)); // left
    world.add(std::make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), lime_lambertian)); // back
    world.add(std::make_shared<quad>(point3( 2,-2, 0), vec3(0, 4, 0), vec3(0, 0, 4), purple_lambertian)); // right
    world.add(std::make_shared<quad>(point3(-2, 2, 0), vec3(4, 0, 0), vec3(0, 0, 4), offwhite_lambertian)); // top
    world.add(std::make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 0, 4), turquoise_lambertian)); // down

    // Light Source
    world.add(std::make_shared<quad>(point3(-0.5, 1.95, 1), vec3(1, 0, 0), vec3(0, 0, 1), light)); // top

    double half_edge1 = 0.5;
    double rad1 = 0.5;
    double rad2 = 0.1;
    double center_base_cube1 = -2 + half_edge1;
    double center_base_sphere1 = -2 + rad1;
    double center_base_sphere2 = -2 + rad2;

    // cube cube(point3 c, double el, std::shared_ptr<material> m) 
    world.add(std::make_shared<cube>(point3(-2 + rad1,center_base_cube1, 2), half_edge1 * 2, metal_mat));
    world.add(std::make_shared<cube>(point3(0, center_base_cube1, half_edge1), half_edge1 * 2, red_lambertian));

    // sphere
    world.add(std::make_shared<sphere>(point3(-2 + rad1, center_base_cube1 + half_edge1 + rad1, 2.3), rad1, glass_mat));
    world.add(std::make_shared<sphere>(point3(1, center_base_cube1, 2), rad1, pink_lambertian));
    for (int i = 1; i < 4; ++i) {
        world.add(std::make_shared<sphere>(point3(-1.5 + rad2 + i/2, center_base_sphere2, 3.1), rad2, purple_lambertian));
    }

    // Build world with bvh
    auto bvh_world = std::make_shared<bvh_node>(world);

    // cam.render(world);
    cam.render(*bvh_world);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Done rendering after " << duration.count() << " microseconds" << std::endl;
    return 0;
}