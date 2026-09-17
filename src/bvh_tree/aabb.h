#ifndef AABB_H
#define AABB_H
#include "core/vec3.h"
#include "core/ray.h"
#include "core/constant.h"
#include <iostream>
class aabb {
    private:
        point3 min_corner;
        point3 max_corner;
        void pad() {
            double delta = 0.0001;
            double half_delta = delta/2;
            if (max_corner.x() - min_corner.x() < delta) {
                min_corner -= vec3(half_delta, 0, 0);
                max_corner += vec3(half_delta, 0, 0);
            }
            if (max_corner.y() - min_corner.y() < delta) {
                min_corner -= vec3(0, half_delta, 0);
                max_corner += vec3(0, half_delta, 0);
            }
            if (max_corner.z() - min_corner.z() < delta) {
                min_corner -= vec3(0, 0, half_delta);
                max_corner += vec3(0, 0, half_delta);
            }
        }
    public:
        aabb() : min_corner(INF, INF, INF), max_corner(-INF, -INF, -INF) {}
        aabb(const point3& min_p, const point3& max_p) {
            min_corner = point3(std::min(min_p.x(), max_p.x()), std::min(min_p.y(), max_p.y()), std::min(min_p.z(), max_p.z()));
            max_corner = point3(std::max(min_p.x(), max_p.x()), std::max(min_p.y(), max_p.y()), std::max(min_p.z(), max_p.z()));
            pad();
        }
        point3 get_max() const {
            return max_corner;
        }
        point3 get_min() const {
            return min_corner;
        }
        point3 get_center() const {
            return (min_corner + max_corner) / 2;
        }
        bool get_hit(const ray& r, double t_max, double t_min) const {
            double t_enter = t_min; 
            double t_exit = t_max;
            for (int i = 0; i < 3; ++i){
                double t_lower = (min_corner[i] - r.origin()[i]) / r.direction()[i];
                double t_upper = (max_corner[i] - r.origin()[i]) / r.direction()[i];
                if (t_lower > t_upper) {
                    std::swap(t_lower, t_upper);
                }
                t_enter = std::max(t_lower, t_enter);
                t_exit = std::min(t_upper, t_exit);
                if (t_exit < t_enter) return false;
            }
            return true;
        }
        int longest_axis() const {
            // return 0 - x, 1 - y, 2 - z
            double longest = -INF;
            int axis = -1;
            for (int i = 0; i < 3; ++i) {
                double dist = max_corner[i] - min_corner[i];
                longest = std::max(longest, dist);
                if (dist == longest) axis = i;
            }
            return axis;
        }
        void show() {
            std::cout << "Min corner is (" << min_corner.x() << ", " << min_corner.y() << ", " << min_corner.z() << ")" << std::endl;
            std::cout << "Max corner is (" << max_corner.x() << ", " << max_corner.y() << ", " << max_corner.z() << ")" << std::endl;
        }
};
inline aabb combine(const aabb& box1, const aabb& box2) {
    point3 min_corner_box1 = box1.get_min();
    point3 max_corner_box1 = box1.get_max();
    point3 min_corner_box2 = box2.get_min();
    point3 max_corner_box2 = box2.get_max();
    double max_x = std::max(max_corner_box1.x(), max_corner_box2.x());
    double max_y = std::max(max_corner_box1.y(), max_corner_box2.y());
    double max_z = std::max(max_corner_box1.z(), max_corner_box2.z());
    double min_x = std::min(min_corner_box1.x(), min_corner_box2.x());
    double min_y = std::min(min_corner_box1.y(), min_corner_box2.y());
    double min_z = std::min(min_corner_box1.z(), min_corner_box2.z());
    return aabb(point3(min_x, min_y, min_z), point3(max_x, max_y, max_z));
}
#endif