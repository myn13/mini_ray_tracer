#include "material.h"
// Materials
inline    auto red_lambertian = std::make_shared<lambertian>(maroon);
inline    auto purple_lambertian = std::make_shared<lambertian>(purple);
inline    auto lime_lambertian = std::make_shared<lambertian>(lime);
inline    auto pink_lambertian = std::make_shared<lambertian>(pastel_pink);
inline    auto turquoise_lambertian = std::make_shared<lambertian>(turquoise);
inline    auto offwhite_lambertian = std::make_shared<lambertian>(off_white);
inline    auto metal_mat = std::make_shared<metal>(metallic, 0.01);
inline    auto glass_mat = std::make_shared<dielectric>(off_white, 1.5);
inline    auto turquoise_box_mat = std::make_shared<lambertian>(turquoise);
inline    auto lime_box_mat = std::make_shared<lambertian>(lime);
inline    auto light = std::make_shared<diffuse_light>(bright_white);