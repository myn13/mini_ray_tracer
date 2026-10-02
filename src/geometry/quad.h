#ifndef QUAD_H
#define QUAD_H
#include "hittable.h"
class quad : public hittable {
    private:
        point3 q;
        vec3 u;
        vec3 v;
        vec3 w;
        double d;
        aabb bbox;
        bool is_interior(double a, double b) const {
            if (a <= 1 && a >= 0 && b <= 1  && b >= 0) return true;
            return false;
        }
    public:
    std::shared_ptr<material> material_ptr;
    vec3 normal;
    quad(point3 point, vec3 u_vec, vec3 v_vec, std::shared_ptr<material> m) : q(point), u(u_vec), v(v_vec), material_ptr(m) {
        normal = unit_vector(cross(u, v));
        d = dot(normal, q);
        w = cross(u, v) / dot(cross(u, v), cross(u, v));
        bbox = bounding_box();
    }
    
    bool get_hit(const ray &r, double t_max, double t_min, hit_record &record) const override {
        vec3 r_ori = r.origin();
        vec3 r_dir = r.direction();
        if (std::fabs(dot(normal, r_dir)) >= 1e-8) {
            double t = (d - dot(normal, r_ori)) / dot(normal, r_dir);
            if (t < t_min || t > t_max) return false;
            point3 hit_point = r.at(t);
            point3 from_q = hit_point - q;
            double a = dot(w, cross(from_q, v));
            double b = dot(w, cross(u, from_q));
            if (!is_interior(a, b)) return false;
            record.t = t;
            record.hit_point = hit_point;
            record.set_outward_normal(r, normal);
            record.mat = material_ptr;
            return true;
        } else {
            std::cout << "Does not hit because zero division\n";
            return false;
        }
    }
    virtual aabb bounding_box() const override {
        auto bbox_diagonal1 = aabb(q, q + u + v);
        auto bbox_diagonal2 = aabb(q + u, q + v);
        return aabb(bbox_diagonal1.get_min(), bbox_diagonal2.get_max());
    }
    void show_quad() {
        point3 qu = q + u;
        point3 qv = q + v;
        point3 quv = q + u + v;
        std::cout << "The quad:" << std::endl;
        q.show();
        qu.show();
        qv.show();
        quv.show();
    }
    point3 sample_point() const {
        double a = random_double(0, 1);
        double b = random_double(0,1);
        return q + a * u + b * v;
    }
    double quad_area() const {
        return cross(u, v).length();
    }
    double pdf_for_direction(point3 origin, vec3 direction, hit_record &record) const {
        ray test_ray = ray(origin, direction);
        if (!get_hit(test_ray, INF, 0.001, record)) {
            return 0;
        }
        double distance_squared = record.t * record.t * direction.length_squared();
        double light_cosine = fabs(dot(direction, record.normal))/direction.length();
        if (light_cosine < 1e-8) {
            return 0;
        }
        return distance_squared / (light_cosine * quad_area());
    }
};
#endif