#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "core/vec3.h"

TEST_CASE("Vec3 operator []","[vec3][operators]") {
    vec3 u(1.0, 2.0, 3.0); 
    double result0 = u[0];
    double result1 = u[1];
    double result2 = u[2];
    REQUIRE(result0 == 1.0);
    REQUIRE(result1 == 2.0);
    REQUIRE(result2 == 3.0);
}

TEST_CASE("Vec3 operator opposite vector", "[vec3][operators]") {
    vec3 u(1.0, 2.0, 3.0); 
    vec3 result = -u;
    REQUIRE(result.x() == -1.0);
    REQUIRE(result.y() == -2.0);
    REQUIRE(result.z() == -3.0); 
}

TEST_CASE("Vec3 operator+= increases", "[vec3][operators]") {
    vec3 u(1.5, -1.5, 5.75);
    vec3 v(1.0, 1.0, 5.0);
    u += v;
    REQUIRE(u.x() == 2.5);
    REQUIRE(u.y() == -0.5);
    REQUIRE(u.z() == 10.75); 
}

TEST_CASE("Vec3 operator-= decreases", "[vec3][operators]") {
    vec3 u(1.5, -1.5, 5.75);
    vec3 v(1.0, 1.0, 5.0);
    u -= v;
    REQUIRE(u.x() == 0.5);
    REQUIRE(u.y() == -2.5);
    REQUIRE(u.z() == 0.75); 
}

TEST_CASE("Vec3 operator*= multiplies", "[vec3][operators]") {
    vec3 u(1.5, -1.5, 5.75);
    u *= 2;
    REQUIRE(u.x() == 3.0);
    REQUIRE(u.y() == -3.0);
    REQUIRE(u.z() == 11.5);
}

TEST_CASE("Vec3 operator/= divides", "[vec3][operators]") {
    vec3 u(1.5, -1.5, 5.75);
    u /= 2;
    REQUIRE(u.x() == 0.75);
    REQUIRE(u.y() == -0.75);
    REQUIRE(u.z() == 2.875);
}

TEST_CASE("Vec3 length", "[vec3][operators]") {
    vec3 u(5.0, 6.0, -1.5);
    double result = u.length();
    u.show();
    REQUIRE(result == std::sqrt(63.25));
}

TEST_CASE("Vec3 length squared", "[vec3][operators]") {
    vec3 u(5.0, 6.0, -1.5);
    double result = u.length_squared();
    u.show();
    REQUIRE(result == 63.25);
}

TEST_CASE("Vec3 is near zero", "[vec3][operators]") {
    vec3 u(1e-11, 1e-12, 123e-19);
    bool result = u.near_zero();
    REQUIRE(result == true);
}

TEST_CASE("Vec3 operator + ", "[vec3][operators]") {
    vec3 u(1.5, -1.5, 5.75);
    vec3 v(-7.25, 5.5, 12.5);
    vec3 result = u + v;
    REQUIRE(result.x() == -5.75);
    REQUIRE(result.y() == 4.0);
    REQUIRE(result.z() == 18.25);
}

TEST_CASE("Vec3 operator - ", "[vec3][operators]") {
    vec3 u(1.5, -1.5, 5.75);
    vec3 v(-7.25, 5.5, 12.5);
    vec3 result = u - v;
    REQUIRE(result.x() == 8.75);
    REQUIRE(result.y() == -7);
    REQUIRE(result.z() == -6.75);
}

TEST_CASE("Vec3 operator * ", "[vec3][operators]") {
    vec3 u(1.5, -1.5, 5.75);
    double scale = 4;
    vec3 result = u * scale;
    REQUIRE(result.x() == 6);
    REQUIRE(result.y() == -6);
    REQUIRE(result.z() == 23);
}

TEST_CASE("Vec3 operator / ", "[vec3][operators]") {
    vec3 u(1.5, -1.5, 5.75);
    double div = 4;
    vec3 result = u / div;
    REQUIRE(result.x() == 0.375);
    REQUIRE(result.y() == -0.375);
    REQUIRE(result.z() == 1.4375);
}

TEST_CASE("Vec3 operator dot ", "[vec3][operators]") {
    vec3 u(1.5, -1.5, 5.75);
    vec3 v(-7.25, 5.5, 12.5);
    double result = dot(u, v);
    REQUIRE(result == 52.75);
}

TEST_CASE("Vec3 operator cross ", "[vec3][operators]") {
    vec3 u(1.5, -1.5, 5.75);
    vec3 v(-7.25, 5.5, 12.5);
    vec3 result = cross(u, v);
    REQUIRE(result.x() == -50.375);
    REQUIRE(result.y() == -60.4375);
    REQUIRE(result.z() == -2.625);
}

TEST_CASE("Vec3 operator angle ", "[vec3][operators]") {
    vec3 u(1.5, -1.5, 5.75);
    vec3 v(-7.25, 5.5, 12.5);
    double result = angle(u, v);
    REQUIRE(std::round(result * 1000) / 1000 == 0.98);
}

TEST_CASE("Vec3 operator unit vector ", "[vec3][operators]") {
    vec3 u(1.5, -1.5, 3);
    vec3 result = unit_vector(u);
    REQUIRE(std::round(result.x() * 1000) / 1000 == 0.408);
    REQUIRE(std::round(result.y() * 1000) / 1000 == -0.408);
    REQUIRE(std::round(result.z() * 1000) / 1000 == 0.816);
}