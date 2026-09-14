#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "geometry/sphere.h"
#include "geometry/cube.h"
TEST_CASE("Sphere hits ray", "[sphere][hittable]") {
    // get_hit(const ray &r, double t_max, double t_min, hit_record &record)
    ray r(point3(0, 0, 0), vec3(0, 0, -1));
    sphere s(point3(0, 0, -1), 0.5);
    hit_record rec;
    bool result = s.get_hit(r, INF, 0.001, rec);
    REQUIRE(result == true);
    REQUIRE(rec.t == 0.5);
    REQUIRE(rec.hit_point.x() == 0.0);
    REQUIRE(rec.hit_point.y() == 0.0);
    REQUIRE(rec.hit_point.z() == -0.5);
    REQUIRE(rec.normal.x() == 0.0);
    REQUIRE(rec.normal.y() == 0.0);
    REQUIRE(rec.normal.z() == 1.0);
    REQUIRE(rec.is_front_face == true);
}

TEST_CASE("Sphere miss ray", "[sphere][hittable]") {
    // get_hit(const ray &r, double t_max, double t_min, hit_record &record)
    ray r(point3(0, 0, 0), vec3(1, 0, 0));
    sphere s(point3(0, 0, -1), 0.5);
    hit_record rec;
    bool result = s.get_hit(r, INF, 0.001, rec);
    REQUIRE(result == false);
}

TEST_CASE("Origin inside sphere", "[sphere][hittable]") {
    // get_hit(const ray &r, double t_max, double t_min, hit_record &record)
    ray r(point3(0, 0, 0), vec3(0, 0, -1));
    sphere s(point3(0, 0, 0), 1.0);
    hit_record rec;
    bool result = s.get_hit(r, INF, 0.001, rec);
    REQUIRE(result == true);
    REQUIRE(rec.t == 1.0);
    REQUIRE(rec.hit_point.x() == 0.0);
    REQUIRE(rec.hit_point.y() == 0.0);
    REQUIRE(rec.hit_point.z() == -1.0);
    REQUIRE(rec.normal.x() == 0.0);
    REQUIRE(rec.normal.y() == 0.0);
    REQUIRE(rec.normal.z() == 1.0);
    REQUIRE(rec.is_front_face == false);
}

TEST_CASE("Cube hits ray", "[ray][hittable]") {
    // get_hit(const ray &r, double t_max, double t_min, hit_record &record)
    ray r(point3(0, 0, 0), vec3(0, 0, -1));
    cube c(point3(0, 0, -1), 1.0);
    hit_record rec;
    bool result = c.get_hit(r, INF, 0.001, rec);
    REQUIRE(result == true);
    REQUIRE(rec.t == 0.5);
    REQUIRE(rec.hit_point.x() == 0.0);
    REQUIRE(rec.hit_point.y() == 0.0);
    REQUIRE(rec.hit_point.z() == -0.5);
    REQUIRE(rec.normal.x() == 0.0);
    REQUIRE(rec.normal.y() == 0.0);
    REQUIRE(rec.normal.z() == 1.0);
    REQUIRE(rec.is_front_face == true);
}

TEST_CASE("Cube misses ray", "[ray][hittable]") {
    // get_hit(const ray &r, double t_max, double t_min, hit_record &record)
    ray r(point3(0, 0, 0), vec3(1, 0, 0));
    cube c(point3(0, 0, -1), 1.0);
    hit_record rec;
    bool result = c.get_hit(r, INF, 0.001, rec);
    REQUIRE(result == false);
}

TEST_CASE("Ray is parallel to cube", "[ray][hittable]") {
    // get_hit(const ray &r, double t_max, double t_min, hit_record &record)
    ray r(point3(0, 0, 0), vec3(1, 0, 0));
    cube c(point3(0, 0, -1), 2.0);
    hit_record rec;
    bool result = c.get_hit(r, INF, 0.001, rec);
    REQUIRE(result == false);
}
