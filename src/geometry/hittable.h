#ifndef HITTABLE_H
#define HITTABLE_H
#include "core/ray.h"
#include "core/constant.h"
class material;
struct hit_record {
    point3 hit_point;
    vec3 normal;
    bool is_front_face;
    double t; // ray parameter
    std::shared_ptr<material> mat;

    inline void set_outward_normal(const ray& r, const vec3& outward_normal) {
       // front face: ray and outward normal point in opposite directions (angle > 90°, dot < 0)
        double dot_ = dot(r.direction(), outward_normal);
        is_front_face = (dot_ < 0.0) ? true : false;
        if (!is_front_face) normal = -outward_normal;
        else normal = outward_normal;
    }
};
class hittable {
    public:
        virtual ~hittable() = default;
        virtual bool get_hit(const ray &r, double t_max, double t_min, hit_record &record) const = 0; //compiler will notice the object missing get_hit
};
#endif