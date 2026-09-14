#ifndef ANGLE_H
#define ANGLE_H

inline double degree_to_radian(double d) {
    return d * M_PI / 180.0;
}

inline double radian_to_degree(double r) {
    return r * 180.0 / M_PI;
}

#endif