#ifndef TILES_H
#define TILES_H
#include <vector>
struct tile {
    // (x,y) is the top left pixel coordinate
    int x; // col
    int y; // row
    int width;
    int height;
};
inline std::vector<tile> compute_tile_list(int img_width, int img_height, int tile_size) {
    std::vector<tile> res;
    for (int i = 0; i < img_height; i += tile_size) {
        tile new_tile;
        for (int j = 0; j < img_width; j += tile_size) {
            new_tile.x = j;
            new_tile.y = i;
            new_tile.width = std::min(tile_size, img_width - j);
            new_tile.height = std::min(tile_size, img_height - i);
            res.push_back(new_tile);
        }
    }
    return res;
}
#endif