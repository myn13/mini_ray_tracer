#ifndef VEC3_H
#define VEC3_H
#include <iostream>
class vec3 {
    private:
        double e[3];
    public:
        vec3(): e{0, 0, 0} {}
        vec3(double x, double y, double z) {
            e[0] = x;
            e[1] = y;
            e[2] = z;
        }
        double x() const { return e[0];}
        double y() const { return e[1];}
        double z() const { return e[2];}
        const double& operator[](int i) const { 
            if (i > 2) {
                throw ("Out of dimension");
            } else {
                return e[i];
            }
        }
        double& operator[](int i) {
            if (i > 2) {
                throw ("Out of dimension");
            } else {
                return e[i];
            }
        }
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        vec3& operator=(vec3 u) {
            e[0] = u.x();
            e[1] = u.y();
            e[2] = u.z();
            return *this;
        }
        vec3& operator+=(vec3 v) {
            e[0] += v.x();
            e[1] += v.y();
            e[2] += v.z();
            return *this;
        }
        vec3& operator-=(vec3 v) {
            e[0] -= v.x();
            e[1] -= v.y();
            e[2] -= v.z();
            return *this;
        }
        vec3& operator*=(double t) {
            e[0] = e[0]*t;
            e[1] = e[1]*t;
            e[2] = e[2]*t;
            return *this;
        }
        vec3& operator/=(double t){
            return *this *= 1/t;
        }
        double length() const {
            return std::sqrt(length_squared());
        }
        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }
        bool near_zero() {
            auto s = 1e-8;
            if (std::fabs(e[0]) < s && std::fabs(e[1]) < s && std::fabs(e[2]) < s ) { return true;}
            return false;
        }
        void show() {
            std::cout << "Vector: (" << e[0] << ", " << e[1] << ", " << e[2] << ")\n";
        }
        double distance_to_origin() {
            return std::sqrt(x() * x() + y() * y() + z() * z()); 
        }
        
};
inline vec3 operator+(vec3 u, vec3 v) {
    return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}
inline vec3 operator-(vec3 u, vec3 v) {
    return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}
inline vec3 operator*(vec3 u, double s) {
    return vec3(u.x() * s, u.y() * s, u.z() * s);
}
inline vec3 operator*(double s, vec3 u) {
    return vec3(u.x() * s, u.y() * s, u.z() * s);
}
inline vec3 operator/(vec3 u, double s) {
    return u * (1.0/s);
}
inline double dot(vec3 u, vec3 v) {
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}
inline vec3 cross(vec3 u, vec3 v) {
    return vec3(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(), u.x() * v.y() - u.y() * v.x());
}
inline double angle(vec3 u, vec3 v) {
    double cos = dot(u, v) / (u.length() * v.length());
    return std::acos(cos); //in radians
}
inline vec3 unit_vector(vec3 u) {
    double ux = u.x() / u.length();
    double uy = u.y() / u.length();
    double uv = u.z() / u.length();
    return vec3(ux, uy, uv);
}
inline vec3 reflect_vector(vec3 vec_in, vec3 normal) {
    return vec_in - 2 * dot(vec_in, normal) * normal;
}
inline vec3 refract(const vec3& uv, double eta_over_eta_prime, const vec3& n) {
    double cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 perp = eta_over_eta_prime * (uv + cos_theta * n);
    vec3 parallel = -n * std::sqrt(std::abs(1 - perp.length_squared()));
    return perp + parallel;
}

inline vec3 normalize(vec3 u) {
    return u / dot(u, u);
}
using point3 = vec3;
#endif