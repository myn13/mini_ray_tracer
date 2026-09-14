#ifndef CAMERA_H
#define CAMERA_H
#include "core/vec3.h"
#include "math/angle.h"
#include "math/random_generator.h"
#include "core/color.h"
#include "materials/material.h"
#include "io/fill_color.cpp"
#include "tiles.h"
#include <fstream>
#include <atomic>
#include <thread>
class camera {
    private:
        vec3 u, v, w; // local coordinate system
        point3 pixel_00_at;
        vec3 pixel_delta_u; 
        vec3 pixel_delta_v;
        int image_height;
        vec3 defocus_disk_u;
        vec3 defocus_disk_v;
        inline void set_image_height() {
            image_height = image_width / aspect_ratio; //pixel
        }
        inline double cam_distance() {
            return (lookfrom - lookat).length();
        }
        inline double viewport_height() {
            double theta = degree_to_radian(vfov / 2);
            return 2 * focus_distance * std::tan(theta);
        }
        inline double viewport_width() {
            return viewport_height() * aspect_ratio;
        }
        inline void set_coordinate() {
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = unit_vector(cross(w, u));
        }
        inline void set_pixel_delta() {
            pixel_delta_u = viewport_width() * u / image_width;
            pixel_delta_v = -viewport_height() * v / image_height;
        }
        void initialize() {
            set_image_height();
            set_coordinate();
            set_pixel_delta();
            auto viewport_u = u * viewport_width();
            auto viewport_v = - v * viewport_height();
            point3 viewport_upper_left = lookfrom - focus_distance * w - viewport_u / 2 - viewport_v / 2;
            pixel_00_at = viewport_upper_left + 0.5 * pixel_delta_u + 0.5 * pixel_delta_v; 
            double defocus_radius = focus_distance * std::tan(degree_to_radian(defocus_angle / 2));
            defocus_disk_u = defocus_radius * u;
            defocus_disk_v = defocus_radius * v;
        }
        point3 defocus_sample() const {
            vec3 random_vector = random_in_unit_disk();
            return (lookfrom + random_vector.x() * defocus_disk_u + random_vector.y() * defocus_disk_v);
        }
        // i is the horizontal step, j is the vertical step
        ray get_ray_at(int i, int j) const {
            vec3 jitter(random_less_than_1() - 0.5, random_less_than_1() - 0.5, 0);
            point3 pixel_sample = pixel_00_at + (i + jitter.x()) * pixel_delta_u + (j + jitter.y()) * pixel_delta_v;
            point3 ray_ori = defocus_angle <= 0 ? lookfrom : defocus_sample();
            vec3 ray_dir = pixel_sample - ray_ori;
            return ray(ray_ori, ray_dir);
        }
        color ray_color(const ray& r, const hittable& world, int depth) {
            if (depth <= 0) {
                // std::cout << "Depth exhausted\n";
                return color(0, 0, 0);
            }
            hit_record rec;
            if (world.get_hit(r, INF, 0.001, rec)) {
                ray scattered_ray;
                color attenuation;
                if(rec.mat->scatter(r, rec, attenuation, scattered_ray)) {
                    color result = attenuation * ray_color(scattered_ray, world, depth - 1);
                    if (std::isnan(result.r) || std::isnan(result.g) || std::isnan(result.b)) {
                        std::cout << "NaN detected!\n";
                    }
                    return result;
                } 
                return color(0, 0, 0);
            } else {
                vec3 unit_dir = unit_vector(r.direction());
                double a = 0.5 * (unit_dir.y() + 1);
                return a * color(0.5, 0.7, 1.0) + (1 - a) * color(1.0, 1.0, 1.0); // simulate the sky gradient
            }
        }
        void render_tile(const tile& ti, const hittable& world, std::vector <uint8_t> &buffer) {
            int tile_w = ti.width;
            int tile_h = ti.height;
            int tile_x = ti.x;
            int tile_y = ti.y;
            for (int r = tile_y; r < tile_y + tile_h; ++r) {
                for (int c = tile_x; c < tile_x + tile_w; ++c) {
                    color m(0, 0, 0);
                    for (int s = 0; s < sample_per_pixel; ++s) {
                        ray ra = get_ray_at(c, r);
                        m += ray_color(ra, world, max_depth);
                    }
                    size_t index = static_cast<size_t>(r * image_width + c ) * 3;
                    write_into_buffer(buffer, index, m, sample_per_pixel);
                }
            }
        }
        void write_buffer_to_ppm(const std::vector<uint8_t>& buffer, int image_width, int image_height, const std::string& output_path) {
            std::ofstream out(output_path);
            if (!out) {
                std::clog << "Failed to open output file: " << output_path << "\n";
                return;
            }
            out << "P3\n" << image_width << ' ' << image_height << "\n255\n";
            for (int row = 0; row < image_height; ++row) {
                for (int col = 0; col < image_width; ++col) {
                    size_t idx = (static_cast<size_t>(row) * image_width + col) * 3;
                    out << static_cast<int>(buffer[idx]) << ' '
                        << static_cast<int>(buffer[idx+1]) << ' '
                        << static_cast<int>(buffer[idx+2]) << '\n';
                }
            }
        }
    public:
        const double aspect_ratio = 16.0/9.0; 
        point3 lookfrom = point3(6.5, 1, 0.5); 
        point3 lookat = point3(0, 0, 0);
        vec3 vup = vec3(0, 1, 0);
        double vfov = 90;
        double focus_distance = 3.4; // world unit
        double defocus_angle = 0;
        int image_width = 400; //pixel
        int sample_per_pixel = 50;
        int max_depth = 50;
        camera() = default;
        camera(point3 lf, point3 la, vec3 v, double vf, double fd, double da, int iw, int spp, int md) {
            lookfrom = lf;
            lookat = la;
            vup = v;
            vfov = vf;
            focus_distance = fd;
            defocus_angle = da;
            image_width = iw;
            sample_per_pixel = spp;
            max_depth = md;
        }
        std::string output_path = "render_world.ppm";
        std::vector<uint8_t> buffer;
        
        void render(const hittable& world) {
            initialize();
            // first version: show ppm image
            std::ofstream out(output_path);
            if (!out) {
                std::clog << "Failed to open output file: " << output_path << "\n";
                return;
            }
            // out << "P3\n" << image_width << ' ' << image_height << "\n255\n";
            // for (int i = 0 ; i < image_height; ++i) {
            //     for (int j = 0; j < image_width; ++j) {
            //         color c(0,0,0);
            //         size_t buffer_index = (static_cast<size_t>(i) * image_width + j) * 3;
            //         for (int h = 0; h < sample_per_pixel; ++h) {
            //             ray r = get_ray_at(j, i);
            //             c += ray_color(r, world, max_depth);
            //         }
            //         // write_color_ppm(out, c, sample_per_pixel);
            //         write_into_buffer(buffer, buffer_index, c, sample_per_pixel);
            //     }
            // }
            // for (int i = 0; i < 101; ++i) {
            //     std::cout << static_cast <int> (buffer[i]) << " ";
            // }
            // make workers
            buffer.resize(image_width * image_height * 3);
            auto tiles = compute_tile_list(image_width, image_height, 32);
            std::atomic<int> next_tile{0};
            auto worker = [&]() {
                while (true) {
                    int idx = next_tile.fetch_add(1);
                    if (idx >= tiles.size()) break;
                    tile current_tile = tiles[idx];
                    render_tile(current_tile, world, buffer);
                }
            };
            unsigned int number_threads = std::thread::hardware_concurrency();
            // unsigned int number_threads = 1;
            std::vector<std::thread> threads;
            for (unsigned int t = 0; t < number_threads; ++t) {
                threads.emplace_back(worker);
            }
            for (auto& th: threads) {
                th.join();
            }
            write_buffer_to_ppm(buffer, image_width, image_height, output_path);
            // std::cout << "Using " << number_threads << " threads\n";
            // std::cout << "Generated " << tiles.size() << " tiles\n";
            // std::cout << std::endl;
        }
};
#endif