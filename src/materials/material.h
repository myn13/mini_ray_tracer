#ifndef MATERIAL_H
#define MATERIAL_H
#include "core/ray.h"
#include "core/color.h"
#include "core/vec3.h"
struct hit_record;

class material {
    public:
        virtual ~material() = default;
        virtual bool scatter(const ray& ray_in, hit_record& rec, color& attenuation, ray& ray_out) const = 0;
};
#endif