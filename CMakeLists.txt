cmake_minimum_required(VERSION 3.16)
project(RayTracer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Catch2)

add_library(raytracer_lib INTERFACE)
target_include_directories(raytracer_lib INTERFACE ${CMAKE_SOURCE_DIR}/src)

add_executable(test_vec3 tests/test_vec3.cpp)
target_link_libraries(test_vec3 PRIVATE raytracer_lib Catch2::Catch2WithMain)

# add_executable(test_hittable tests/test_hittable.cpp)
# target_link_libraries(test_hittable PRIVATE raytracer_lib Catch2::Catch2WithMain)

# add_executable(fill_color src/io/fill_color.cpp)
# target_include_directories(fill_color PRIVATE ${CMAKE_SOURCE_DIR}/src/io)

add_executable(main main.cpp)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/src)