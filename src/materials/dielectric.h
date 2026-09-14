#ifndef DIELECTRIC_H
#define DIELECTRIC_H
#include "material.h"
#include "core/vec3.h"
#include "math/utils.cpp"
class dielectric: public material {
    private:
        color albedo;
        double index_refraction;
    public:
        dielectric(class color c, double r): albedo(c), index_refraction(r) {}
        bool scatter(const ray& ray_in, hit_record& rec, color& attenuation, ray& ray_out) const override {
            double refraction_ratio;
            if (rec.is_front_face) refraction_ratio = 1.0 / index_refraction; // 1.0 is from air
            else refraction_ratio = index_refraction;
            double angle = dot(-unit_vector(ray_in.direction()), rec.normal);
            double cosine = std::cos(degree_to_radian(angle));
            double sine = std::sqrt(1 - cosine * cosine);
            vec3 ref;
            if (sine / refraction_ratio <= 1) {
                ref = refract(unit_vector(ray_in.direction()), refraction_ratio, rec.normal);
            } else if (sine / refraction_ratio > 1 || schlick(angle, index_refraction) > random_less_than_1()) {
                ref = reflect_vector(unit_vector(ray_in.direction()), rec.normal);
            }
            ray_out = ray(rec.hit_point, ref);
            attenuation = albedo;
            return true;
        }
};
#endif