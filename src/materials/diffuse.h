#ifndef DIFFUSE_H
#define DIFFUSE_H
#include "material.h"
class diffuse_light : public material {
    private: 
        color emission;
    public:
        diffuse_light(color e) : emission(e) {}
        bool scatter(const ray& ray_in, hit_record& rec, color& attenuation, ray& ray_out) const override { return false;}
        color emitted(double u, double v, const point3& p) const override {
            return emission;
        }
};
#endif