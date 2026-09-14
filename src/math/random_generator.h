#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H
#include <random>
#include "core/vec3.h"
class random_generator {
    private:
        std::mt19937 engine;
        std::uniform_real_distribution<double> dist;
    public:
        random_generator() {
            // get seed
            std::random_device rand{};
            engine = std::mt19937(rand());
            dist = std::uniform_real_distribution<double>(0.0, 1.0);
        }
        double next_double() {
            return dist(engine);
        }
        double next_double(double min, double max) {
            return min + (max - min) * next_double();
        }
};
inline random_generator& my_random_generator() {
    thread_local random_generator rand_generator;
    return rand_generator;
}
inline double random_less_than_1() {
    return my_random_generator().next_double();
}
inline double random_double(double min, double max) {
    return my_random_generator().next_double(min, max);
}
inline vec3 random_in_unit_disk() {
    while (true) {
        vec3 temp(random_double(-1, 1), random_double(-1, 1), 0);
        if (temp.length_squared() < 1) {
            return temp;
        }
    }
}
inline vec3 random_unit_vector() {
    while (true) {
        vec3 temp(random_double(-1, 1), random_double(-1, 1), random_double(-1, 1));
        if (temp.length_squared() < 1) {
            return temp;
        }
    }
}
#endif