#ifndef RAY_H
#define RAY_H
#include "vec3.h"
class ray {
    private:
        point3 ori;
        vec3 dir;
    public:
        ray() {}
        ray(const point3& o, const point3& d) : ori(o), dir(d) {}
        point3 origin() const { return ori; }
        vec3 direction() const {return dir; }
        point3 at(double t) const {
            return ori + t * dir;
        }
};
#endif