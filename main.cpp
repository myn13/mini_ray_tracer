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
int main() {
    color maroon = decimal_to_color(128, 0, 0);
    color teal = decimal_to_color(0, 128, 128);
    color turquoise = decimal_to_color(128, 0, 0);
    color sky = decimal_to_color(135, 206, 235);
    color sienna = decimal_to_color(160, 82, 45);

    // Materials
    auto ground = std::make_shared<lambertian>(sienna);
    auto lamb_mat = std::make_shared<lambertian>(maroon);
    auto metal_mat = std::make_shared<metal>(teal, 0.01);
    auto glass_mat = std::make_shared<dielectric>(sky, 1.5);

    // World
    hittable_list world;
    
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100.0, ground));
    world.add(std::make_shared<sphere>(point3(-0.5, 0, -1), 0.75, lamb_mat));
    // for (size_t i = 1; i < 101; i += 5) { // 40 spheres
    //     world.add(std::make_shared<sphere>(point3(-0.5, i/2, -1), 0.75, lamb_mat));
    //     world.add(std::make_shared<sphere>(point3(0 + i/5, -100.5, -1 + i/5), 100.0, lamb_mat));
    // }
    world.add(std::make_shared<sphere>(point3(1.5, 0, 0), 0.75, metal_mat));
    // for (size_t i = 1; i < 11; ++i) { //10
    //     world.add(std::make_shared<sphere>(point3(1.5, i, 0), 0.75, metal_mat));
    // }
    world.add(std::make_shared<sphere>(point3(2, 0, 2), 0.75, glass_mat));
    // for (size_t i = 1; i < 11; ++i) { //10
    //     world.add(std::make_shared<sphere>(point3(2, 2 * i, 2), 0.75, glass_mat));
    // }
    world.add(std::make_shared<cube>(point3(0, 0, -3), 0.75, lamb_mat));
    // for (size_t i = 1; i < 11; ++i) {
    //     world.add(std::make_shared<cube>(point3(0, i, -3), 0.75, lamb_mat));
    //     world.add(std::make_shared<cube>(point3(i/10, 0, -3 + i / 2), 0.75, lamb_mat));
    // }
    world.add(std::make_shared<cube>(point3(0, 0, -5), 1, metal_mat));
    // for (size_t i = 1; i < 101; i += 5) {
    //     world.add(std::make_shared<cube>(point3(0, i / 2, -5), 1, metal_mat));
    // }
    world.add(std::make_shared<cube>(point3(2, 0, -3), 1, glass_mat));
    // for (size_t i = 1; i < 51; i += 5) {
    //     world.add(std::make_shared<cube>(point3(2 + i, 0, -3 - i), 1, glass_mat));
    // }

    // Build world with bvh
    // std::cout << "World has" << world.objects.size() << std::endl;
    auto bvh_world = std::make_shared<bvh_node>(world);

    // Camera
    camera cam;
    std::vector<int> values(10000);
    auto start = std::chrono::high_resolution_clock::now();
    // cam.render(world);
    cam.render(*bvh_world);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Done rendering after " << duration.count() << " microseconds" << std::endl;
    return 0;
}