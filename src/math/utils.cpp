#include "angle.h"
inline double schlick(double angle, double refractive_index) {
    // assume the ray from air so refractive of air is 1
    double r0 = (1 - refractive_index) * (1 - refractive_index) / ((1 + refractive_index) * (1 + refractive_index));
    return r0 + (1 - r0) * std::pow(1 - std::cos(degree_to_radian(angle)), 5);
}