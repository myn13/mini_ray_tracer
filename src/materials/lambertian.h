#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H
#include "material.h"
#include "geometry/hittable.h"
#include "core/color.h"
class lambertian: public material {
    private:
        color albedo;
    public:
        lambertian(class color c) : albedo(c){};
        bool scatter(const ray& ray_in, hit_record& rec, color& attenuation, ray& ray_out) const override {
            vec3 scatter_direction = rec.normal + random_unit_vector();
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }
            ray_out = ray(rec.hit_point, scatter_direction);
            attenuation = albedo;
            return true;
        } 
};
#endif