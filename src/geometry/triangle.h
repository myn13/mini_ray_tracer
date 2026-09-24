#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "core/vec3.h"
#include "core/ray.h"
#include "hittable.h"
class triangle : public hittable {
    private:
        point3 v0;
        point3 v1;
        point3 v2;
        vec3 normal;
        double d;
        aabb bbox;
        std::shared_ptr<material> material_ptr;
        bool is_interior(double a, double b) const {
            if (a >= 0 && b >= 0 && a + b <= 1) return true;
            return false;
        }
    public:
        triangle(point3 p1, point3 p2, point3 p3) : v0(p1), v1(p1), v2(p2) {
            vec3 v01 = v1 - v0;
            vec3 v02 = v2 - v0;
            vec3 n = cross(v01, v12);
            normal = unit_vector(n);
            d = dot(n, v0);
            bbox = bounding_box();
        }
        virtual bool get_hit(const ray &r, double t_max, double t_min, hit_record &record) const override {
            vec3 r_ori = r.origin();
            vec3 r_dir = r.direction();
            if (std::fabs(dot(normal, r_dir)) < 1e-8) return false;
            double t = (d - dot(normal, r_ori) ) / dot(normal, r_dir);
            if (t < t_min || t > t_max) return false;
            point3 hit_point = r.at(t);
            vec3 e0 = v1 - v0;
            vec3 e1 = v2 - v1;
            vec3 e2 = v2 - v0;
            vec3 w = cross(e0, e2) / dot(cross(e0, e2), cross(e0, e2));
            vec3 from_ori = hit_point - v0;
            double u = dot(w, cross(from_ori, e2));
            double v = dot(w, cross(e0, from_ori));
            if (!is_interior(u, v)) return false;
            record.t = t;
            record.hit_point = hit_point;
            record.set_outward_normal(r, normal);
            record.mat = material_ptr;
            return true;
        }
        virtual aabb bounding_box() const override {
            aabb bbox1(v0, v1);
            aabb bbox2(v1, v2);
            return combine(bbox1, bbox2);
        }
};
#endif