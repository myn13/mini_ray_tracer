#include "tiles.h"
#include <iostream>
int main() {
    std::vector<tile> list = compute_tile_list(5, 10, 3);
    for (const auto& t: list) {
        std::cout << t.x << ", " << t.y << ", " << t.width << ", " << t.height << std::endl;
    }
    return 0;
}