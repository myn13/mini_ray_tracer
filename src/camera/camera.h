#ifndef CAMERA_H
#define CAMERA_H
#include "core/vec3.h"
#include "math/angle.h"
#include "math/random_generator.h"
#include "core/color.h"
#include "materials/material.h"
#include "materials/lambertian.h"
#include "io/fill_color.cpp"
#include "geometry/quad.h"
#include "tiles.h"
#include <fstream>
#include <atomic>
#include <thread>
#include <cmath>
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
            point3 ray_ori = defocus_angle <= 0.01 ? lookfrom : defocus_sample();
            vec3 ray_dir = pixel_sample - ray_ori;
            return ray(ray_ori, ray_dir);
        }
        vec3 cosine_sample_hemisphere(const vec3& n) const {
            vec3 local = random_cosine_direction();
            vec3 w = n;
            vec3 a = (std::fabs(w.x()) > 0.9) ? vec3(0, 1, 0) : vec3(1, 0, 0);
            vec3 v = unit_vector(cross(w, a));
            vec3 u = cross(w, v);
            return unit_vector(u * local.x() + v * local.y() + w * local.z());
        }
        double cosine_pdf(const vec3& n, const vec3& direction) const {
            double cosine = dot(n, unit_vector(direction));
            return cosine <= 0 ? 0 : cosine / PI;
        }
        color finite_or_black(const color& c) const {
            if (!std::isfinite(c.r) || !std::isfinite(c.g) || !std::isfinite(c.b)) {
                return color(0, 0, 0);
            }
            return c;
        }
        color direct_light_contribution(const hit_record& rec, const vec3& surface_normal, std::shared_ptr<lambertian>& mat, const hittable& world) {
            if (!light_quad) {
                return color(0, 0, 0);
            }
            point3 light_point = light_quad->sample_point();
            vec3 to_light = light_point - rec.hit_point;
            double distance = to_light.length();
            if (distance < 1e-6) {
                return color(0, 0, 0);
            }
            vec3 light_direction = to_light / distance;
            double surface_cosine = std::max(0.0, dot(surface_normal, light_direction));
            double light_cosine = std::fabs(dot(light_quad->normal, -light_direction));
            if (surface_cosine <= 0 || light_cosine < 1e-8) {
                return color(0, 0, 0);
            }
            hit_record shadow_record;
            ray shadow_ray(rec.hit_point, light_direction);
            if (world.get_hit(shadow_ray, distance - 0.001, 0.001, shadow_record)) {
                return color(0, 0, 0);
            }
            double distance_squared = distance * distance;
            distance_squared = std::max(distance * distance, 0.7);
            double pdf = distance_squared / (light_cosine * light_quad->quad_area());
            if (pdf <= 0) {
                return color(0, 0, 0);
            }
            color brdf_value = mat->get_albedo() / PI;
            color emitted = light_quad->material_ptr->emitted(shadow_ray, rec, 0, 0, light_point);
            return emitted * brdf_value * surface_cosine / pdf;
        }
        color ray_color(const ray& r, const hittable& world, int depth, bool count_emission = true) {
            if (depth <= 0) {
                return color(0, 0, 0);
            }
            hit_record rec;
            if (world.get_hit(r, INF, 0.001, rec)) {
                ray scattered_ray;
                color attenuation;
                color color_from_emission = count_emission ? rec.mat->emitted(r, rec, 0.0, 0.0, rec.hit_point) : color(0, 0, 0);
                auto lambert_mat = std::dynamic_pointer_cast<lambertian>(rec.mat);
                if (lambert_mat) {
                    color direct = direct_light_contribution(rec, rec.normal, lambert_mat, world);
                    vec3 scatter_dir = cosine_sample_hemisphere(rec.normal);
                    double pdf = cosine_pdf(rec.normal, scatter_dir);
                    double surface_cosine = std::max(0.0, dot(rec.normal, scatter_dir));
                    if (pdf <= 0 || surface_cosine <= 0) {
                        return finite_or_black(color_from_emission + direct);
                    }
                    scattered_ray = ray(rec.hit_point, scatter_dir);
                    color brdf_value = lambert_mat->get_albedo() / PI;
                    color indirect = brdf_value * surface_cosine * ray_color(scattered_ray, world, depth - 1, false) / pdf;
                    return finite_or_black(color_from_emission + direct + indirect);
                }
                if (rec.mat->scatter(r, rec, attenuation, scattered_ray)) {
                    return finite_or_black(attenuation * ray_color(scattered_ray, world, depth - 1, true) + color_from_emission);
                }
                return color_from_emission;
            } else {
                return background;
            }
        }
        float compute_color_variance(std::vector<color>& sample_colors) {
            if (sample_colors.size() <= 1) return 0.0f;
            float n = static_cast<float> (sample_colors.size());
            float mean_r = 0.0f;
            float mean_g = 0.0f;
            float mean_b = 0.0f;
            for (const auto& col: sample_colors) {
                mean_r += col.r;
                mean_g += col.g;
                mean_b += col.b;
            }
            mean_r /= n;
            mean_g /= n;
            mean_b /= n;

            float var_r = 0.0f, var_g = 0.0f, var_b = 0.0f;
            for (const auto& col : sample_colors) {
                float dr = col.r - mean_r;
                float dg = col.g - mean_g;
                float db = col.b - mean_b;
                
                var_r += dr * dr;
                var_g += dg * dg;
                var_b += db * db;
            }
            
            var_r /= (n - 1);
            var_g /= (n - 1);
            var_b /= (n - 1);
            return (var_r + var_g + var_b) / 3.0f;
        }
        void render_tile(const tile& ti, const hittable& world, std::vector <uint8_t> &buffer) {
            int tile_w = ti.width;
            int tile_h = ti.height;
            int tile_x = ti.x;
            int tile_y = ti.y;

            int min_spp = 8;
            int max_spp = sample_per_pixel;
            int batch_size = 8;
            float error_threshold = 0.002f;

            for (int r = tile_y; r < tile_y + tile_h; ++r) {
                for (int c = tile_x; c < tile_x + tile_w; ++c) {
                    color m(0, 0, 0);
                    std::vector<color> sample_colors;
                    sample_colors.reserve(max_spp);
                    int samples_taken = 0;
                    for (int s = 0; s < max_spp; s += batch_size) {
                        for (int b = 0; b < batch_size && (s + b) < max_spp; ++b) {
                            ray ra = get_ray_at(c, r);
                            color sample_color = ray_color(ra, world, max_depth);
                            m += sample_color;
                            sample_colors.push_back(sample_color);
                            samples_taken++;
                        }
                        if (samples_taken >= min_spp) {
                            float current_variance = compute_color_variance(sample_colors);
                            if (current_variance < error_threshold) {
                                break;
                            }
                        }
                    }
                    
                    size_t index = static_cast<size_t>(r * image_width + c ) * 3;
                    write_into_buffer(buffer, index, m, samples_taken);
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
        double aspect_ratio = 1.0; 
        point3 lookfrom = point3(278, 278, -800); 
        point3 lookat = point3(278, 278, 0);
        vec3 vup = vec3(0, 1, 0);
        double vfov = 40;
        double focus_distance = 3.4; // world unit
        double defocus_angle = 0;
        int image_width = 1200; //pixel
        int sample_per_pixel = 32;
        int max_depth = 50;
        color background = color(0.7, 0.8, 1.0);
        std::shared_ptr<quad> light_quad; 
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
        std::vector<uint8_t> buffer;
        void render(const hittable& world, std::string output_path) {
            initialize();
            std::ofstream out(output_path);
            if (!out) {
                std::clog << "Failed to open output file: " << output_path << "\n";
                return;
            }
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
        }
};
#endif