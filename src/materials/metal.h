#ifndef METAL_H
#define METAL_H
#include "material.h"
class metal: public material {
    private:
        color albedo;
        double fuzz;
    public:
        metal(color c, double f): albedo(c) {
            fuzz = f < 1 ? f : 1.0;
        }
        bool scatter(const ray& ray_in, hit_record& rec, color& attenuation, ray& ray_out) const override {
            vec3 reflect_vec = reflect_vector(unit_vector(ray_in.direction()), rec.normal);
            vec3 fuzz_direction = fuzz * random_unit_vector();
            ray_out = ray(rec.hit_point, reflect_vec + fuzz_direction);
            attenuation = albedo;
            return (dot(ray_out.direction(), rec.normal) > 0);
        }
};
#endif