#ifndef DIELECTRIC_H
#define DIELECTRIC_H
#include "material.h"
#include "core/vec3.h"
#include "math/random_generator.h"
#include <cmath>
class dielectric: public material {
    private:
        color albedo;
        double index_refraction;
    public:
        dielectric(class color c, double r): albedo(c), index_refraction(r) {}
        static double schlick_cosine(double cosine, double refraction_ratio) {
            double r0 = (1.0 - refraction_ratio) / (1.0 + refraction_ratio);
            r0 = r0 * r0;
            return r0 + (1.0 - r0) * std::pow(1.0 - cosine, 5.0);
        }
        bool scatter(const ray& ray_in, hit_record& rec, color& attenuation, ray& ray_out) const override {
            double refraction_ratio = rec.is_front_face ? (1.0 / index_refraction) : index_refraction;
            vec3 unit_in = unit_vector(ray_in.direction());
            double cos_theta = std::fmin(dot(-unit_in, rec.normal), 1.0);
            double sin_theta = std::sqrt(std::max(0.0, 1.0 - cos_theta * cos_theta));
            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 ref;
            if (cannot_refract || schlick_cosine(cos_theta, refraction_ratio) > random_less_than_1()) {
                ref = reflect_vector(unit_in, rec.normal);
            } else {
                ref = refract(unit_in, refraction_ratio, rec.normal);
            }
            ray_out = ray(rec.hit_point, ref);
            attenuation = albedo;
            return true;
        }
        color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const override { return color(0, 0, 0); }
};
#endif