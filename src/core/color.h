#ifndef COLOR_H
#define COLOR_H
class color {
    public:
        double r;
        double g;
        double b;
        color() {}
        color(double red, double green, double blue) {
            r = red;
            g = green;
            b = blue;
        }
        color& operator+=(color v) {
            r += v.r;
            g += v.g;
            b += v.b;
            return *this;
        }
        color& operator-=(color v) {
            r -= v.r;
            g -= v.g;
            b -= v.b;
            return *this;
        }
        color& operator*=(double v) {
            r *= v;
            g *= v;
            b *= v;
            return *this;
        }
        color& operator/=(double v) {
            return *this *= (1 / v);
        }
};

inline color operator+(color u, color v) {
    return color(u.r + v.r, u.g + v.g, u.b + v.b);
}
inline color operator-(color u, color v) {
    return color(u.r - v.r, u.g - v.g, u.b - v.b);
}
inline color operator*(color u, double s) {
    return color(u.r * s, u.g * s, u.b * s);
}
inline color operator*(double s, color u) {
    return color(u.r * s, u.g * s, u.b * s);
}
inline color operator*(color u, color v) {
    return color(u.r * v.r, u.g * v.g, u.b * v.b);
}
inline color operator/(color u, double s) {
    return u * (1.0/s);
}
inline color decimal_to_color(int r, int g, int b) {
    return color(r/255.0, g/255.0, b/255.0);
}
#endif 