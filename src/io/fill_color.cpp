#include "../core/color.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>
inline void write_color_ppm(std::ostream& out, const color& c, int sample_per_pixel){
    float red = c.r;
    float green = c.g;
    float blue = c.b;
    double scale = 1.0 / sample_per_pixel;
    red = std::sqrt(red * scale);
    green = std::sqrt(green * scale);
    blue = std::sqrt(blue * scale);
    red = std::clamp(red, 0.0f, 0.999f);
    green = std::clamp(green, 0.0f, 0.999f);
    blue = std::clamp(blue, 0.0f, 0.999f);
    out << static_cast<int> (255 * red) << " " << static_cast<int> (255 * green) << " " << static_cast<int> (255 * blue) << "\n";
}

inline void write_into_buffer(std::vector<uint8_t>& buffer, size_t index, const color& c, int sample_per_pixel){
    float red = c.r;
    float green = c.g;
    float blue = c.b;
    if (!std::isfinite(red) || red < 0) red = 0;
    if (!std::isfinite(green) || green < 0) green = 0;
    if (!std::isfinite(blue) || blue < 0) blue = 0;
    double scale = 1.0 / sample_per_pixel;
    red = std::sqrt(red * scale);
    green = std::sqrt(green * scale);
    blue = std::sqrt(blue * scale);
    red = std::clamp(red, 0.0f, 0.999f);
    green = std::clamp(green, 0.0f, 0.999f);
    blue = std::clamp(blue, 0.0f, 0.999f);

    buffer[index] = static_cast<uint8_t> (255 * red);
    buffer[index + 1] = static_cast<uint8_t> (255 * green);
    buffer[index + 2] = static_cast<uint8_t> (255 * blue);
}

