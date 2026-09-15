#include "aabb.h"
#include <iostream>
int main() {
    aabb test_aabb1 = aabb(point3(0, 0, 1), point3(2, 4, 21));
    std::cout << test_aabb1.longest_axis() << std::endl;
    aabb test_aabb2 = aabb(point3(-11, -3, 7), point3(2, 2, 2));
    aabb combined = combine(test_aabb1, test_aabb2);
    combined.show();
}