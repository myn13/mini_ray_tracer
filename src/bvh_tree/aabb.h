#ifndef AABB_H
#define AABB_H
#include "core/vec3.h"
#include "core/constant.h"
class aabb {
    private:
        point3 min_corner;
        point3 max_corner;
        void pad() {
            
        }
    public:
        aabb() : min_corner(INF, INF, INF), max_corner(INF, INF, INF) {}
        aabb(point3 min, point3 max) {

        }
        point3 get_max() {

        }
        point3 get_min() {

        }
        bool get_hit() {
            ///
            return false;
        }
        int longest_axis() {

        }
};
inline aabb combine(aabb box) {
    return;
}
#endifs