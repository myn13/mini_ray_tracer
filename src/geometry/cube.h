#ifndef CUBE_H
#define CUBE_H
#include "core/vec3.h"
#include "hittable.h"
class cube: public hittable {
    public:
        cube(point3 c, double el, std::shared_ptr<material> m) : center(c), edge_length(el), material_ptr(m) { set_boundary(); }
        virtual bool get_hit(const ray &r, double t_max, double t_min, hit_record &record) const override {
            std::vector<double> min_boundary{x_min, y_min, z_min};
            std::vector<double> max_boundary{x_max, y_max, z_max};
            double t_enter = t_min;
            double t_exit = t_max;
            int hit_axis = -1; // 0 - x, 1 - y, 2 - z
            double hit_sign = 1.0;
            for (int i = 0; i < 3; ++i){
                double t_low = (min_boundary[i] - r.origin()[i] ) / r.direction()[i];
                double t_high = (max_boundary[i] - r.origin()[i]) / r.direction()[i];
                if (r.direction()[i] < 0) std::swap(t_low, t_high);
                if (t_enter < t_low) {
                    t_enter = t_low;
                    hit_axis = i;
                    hit_sign = r.direction()[i] < 0 ? 1.0 : -1.0;
                }
                // t_enter = std::max(t_enter, t_low);
                t_exit = std::min(t_exit, t_high);
                if (t_exit <= t_enter) {
                    // std::cout << "Ray dimisses\n";
                    return false;
                }
            }
            if (hit_axis == -1) return false;
            // std::cout << "T enter is " << t_enter << std::endl;
            // std::cout << "T exit is " << t_exit<< std::endl;
            // std::cout << "Ray hits\n";
            record.t = t_enter;
            record.hit_point = r.at(record.t);
            vec3 outward_normal(0, 0, 0);
            outward_normal[hit_axis] = hit_sign;
            record.set_outward_normal(r, outward_normal);
            record.mat = material_ptr;
            return true;
        }
        virtual aabb bounding_box() const override {
            return aabb(point3(x_min, y_min, z_min), point3(x_max, y_max, z_max));
        }
    private:
        point3 center;
        double edge_length;
        std::shared_ptr<material> material_ptr;
        double x_max, x_min, y_max, y_min, z_max, z_min;
        void set_boundary() {
            double half = edge_length / 2;
            x_min = center.x() - half;
            x_max = center.x() + half;
            y_min = center.y() - half;
            y_max = center.y() + half;
            z_min = center.z() - half;
            z_max = center.z() + half;
            // std::cout << "x in range of " << x_min << " " << x_max << std::endl;
            // std::cout << "y in range of " << y_min << " " << y_max << std::endl;
            // std::cout << "z in range of " << z_min << " " << z_max << std::endl;
        }
};
#endif