#ifndef DIFFUSE_H
#define DIFFUSE_H
#include "material.h"
class diffuse_light : public material {
    private: 
        color emission;
    public:
        diffuse_light(color e) : emission(e) {}
        bool scatter(const ray& ray_in, hit_record& rec, color& attenuation, ray& ray_out) const override { return false;}
        color emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const override {
            if (!rec.is_front_face) return color(0, 0, 0);
            return emission;
        }
};
#endif