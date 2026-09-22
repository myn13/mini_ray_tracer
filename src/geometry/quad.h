#ifndef QUAD_H
#define QUAD_H
#include "hittable.h"
class quad : public hittable {
    private:
        point3 q;
        vec3 u;
        vec3 v;
        std::shared_ptr<material> material_ptr;
        aabb bbox;
        bool is_interior(const point3& p, const vec3& normal) const {
            vec3 pq = p - q;
            double a = dot(normalize(normal), cross(pq, v));
            double b = dot(normalize(normal), cross(pq, u));
            if (a <= 1 && a >= 0 && b <= 1  && b >= 0) return true;
            return false;
        }
    public:
    quad(point3 point, vec3 u_vec, vec3 v_vec, std::shared_ptr<material> m) : q(point), u(u_vec), v(v_vec), material_ptr(m) {
        bbox = bounding_box();
    }
    
    bool get_hit(const ray &r, double t_max, double t_min, hit_record &record) const override {
        vec3 n = unit_vector(cross(u, v));
        double d = dot(n, q);
        vec3 r_ori = r.origin();
        vec3 r_dir = r.direction();
        if (dot(n, r_dir) != 0) {
            record.t = (d - dot(n, r_ori)) / dot(n, r_dir);
            if (record.t < t_min || record.t > t_max) return false;
            record.hit_point = r.at(record.t);
            point3 from_q = record.hit_point = q;
            if (angle(u, from_q) > angle(u, v)) return false;
            if (!is_interior(record.hit_point, n)) return false;
            record.set_outward_normal(r, n);
            record.mat = material_ptr;
            return true;
        } else {
            return false;
        }
    }
    aabb bounding_box() const override {
        point3 qu = q + u;
        point3 qv = q + v;
        point3 quv = q + u + v;
        double min_x = std::min({qu.x(), qv.x(), quv.x()});
        double min_y = std::min({qu.y(), qv.y(), quv.y()});
        double min_z = std::min({qu.z(), qv.z(), quv.z()});
        double max_x = std::max({qu.x(), qv.x(), quv.x()});
        double max_y = std::max({qu.y(), qv.y(), quv.y()});
        double max_z = std::max({qu.z(), qv.z(), quv.z()});
        return aabb(point3(min_x, min_y, min_z), point3(max_x, max_y, max_z));
    }
};

#endif