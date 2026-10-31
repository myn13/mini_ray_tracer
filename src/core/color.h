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
inline color maroon = decimal_to_color(128, 0, 0);
inline color metallic = decimal_to_color(204,204,204);
inline color turquoise = decimal_to_color(64, 224, 208);
inline color lime = decimal_to_color(50, 205, 50);
inline color sky = decimal_to_color(135, 206, 235);
inline color sienna = decimal_to_color(160, 82, 45);
inline color pastel_pink = decimal_to_color(248, 200, 220);
inline color purple = decimal_to_color(147, 112, 219);
inline color off_white = decimal_to_color(250, 249, 246);
inline color bright_white(4, 3, 3);
#endif 