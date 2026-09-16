#ifndef SPHERE_H
#define SPHERE_H
#include "core/vec3.h"
#include "hittable.h"
class sphere : public hittable {
    private:
        point3 center;
        double radius;
        std::shared_ptr<material> material_ptr;
    public:
        sphere(point3 c, double r, std::shared_ptr<material> m) : center(c), radius(r), material_ptr(m) {}
        sphere(point3 c, double r) : center(c), radius(r) {}
        point3 get_center() { return center; }
        double get_radius() {return radius; }
        virtual bool get_hit(const ray &r, double t_max, double t_min, hit_record &record) const override {
            // dot(a + bt - c)(a + bt - c) = r^2 -> find t 
            vec3 ray_dir = r.direction();
            vec3 ray_ori = r.origin();
            // std::cout << "Testing ray: origin=" << ray_ori.x() << "," << ray_ori.y() << "," << ray_ori.z()
        //   << " dir=" << ray_dir.x() << "," << ray_dir.y() << "," << ray_dir.z() << "\n";
            // oc = a - c with a: ray origin and c: center of the sphere -> origin minus center
            point3 oc = ray_ori - center;
            double ocb = 2 * dot(oc, ray_dir);
            double ocr = dot(oc, oc) - radius * radius;
            double b2 = dot(ray_dir, ray_dir);
            //b2*t^2 - ocb*t + ocr = 0
            double discriminant = ocb * ocb - 4 * b2 * ocr;
            // std::cout << "discriminant=" << discriminant << "\n";
            if (discriminant < 0) {
                // std::cout << "Ray dismisses\n";
                return false;
            }
            double root = (-ocb - std::sqrt(discriminant)) / (2 * b2);
            if (root < t_min || root > t_max) {
                root = (-ocb + std::sqrt(discriminant)) / (2 * b2);
                if (root < t_min || root > t_max) return false;
            }
            // std::cout << "Ray hits successully\n";
            record.t = root;
            point3 hit_point = r.at(record.t);
            record.hit_point = hit_point;
            vec3 outward_vec = (hit_point - center) / radius;
            record.set_outward_normal(r, outward_vec);
            record.mat = material_ptr;
            // std::cout << "hit t=" << root << "\n";
            return true;
        }
        virtual aabb bounding_box() const override {
            point3 min_corner = point3(center.x() - radius, center.y() - radius, center.z() - radius);
            point3 max_corner = point3(center.x() + radius, center.y() + radius, center.z() + radius);
            return aabb(min_corner, max_corner);
        }
};
#endif