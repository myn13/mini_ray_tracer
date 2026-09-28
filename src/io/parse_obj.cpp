#include <fstream>
#include <sstream>
#include "core/vec3.h"
#include "geometry/triangle.h"
#include "geometry/quad.h"
#include "materials/lambertian.h"
#include "core/color.h"
struct face_vertex {
    int v = -1;
    int vt = -1;
    int vn = -1;
};
// 18/15/10 
inline face_vertex parse_face_vertex(const std::string& token) {
    std::stringstream ss(token);
    face_vertex face_vert;
    std::string x, y, z;
    std::string part;
    if (std::getline(ss, part, '/')) {
        face_vert.v = std::stoi(part);
    }
    if (std::getline(ss, part, '/')) {
        face_vert.vt = std::stoi(part);
    }
    if (std::getline(ss, part, '/')) {
        face_vert.vn = std::stoi(part);
    }
    return face_vert;
}
inline std::vector<std::shared_ptr<hittable>> read_obj(std::ifstream& infile, std::shared_ptr <material> mat){
    std::vector<point3> vertex_list;
    std::vector<std::shared_ptr<hittable>> triangles;
    std::vector<vec3> normal_list;
    if (!infile) {
        std::cerr << "Failed to open file " << std::endl;
        return triangles;
    }
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream ss(line);
        std::string tag;
        ss >> tag;
        if (tag == "v") { // vertex
            double x, y, z;
            ss >> x >> y >> z; //point3
            vertex_list.push_back(point3(x, y, z));
        } else if (tag == "vn") {
            double x, y, z;
            ss >> x >> y >> z; 
            normal_list.push_back(vec3(x, y, z));
        } else if (tag == "vt") {
            double u, v;
            ss >> u >> v;
        } else if (tag == "f") { // face
            std::string token;
            std::vector<face_vertex> face_vertices;
            // 18/15/10 24/14/10 32/17/10
            while (ss >> token) {
                face_vertices.push_back(parse_face_vertex(token));
            }
            if (face_vertices.size() == 3) {
                for (size_t i = 1; i < face_vertices.size() - 1; ++i) {
                    auto tri = std::make_shared<triangle>(vertex_list[face_vertices[0].v - 1], vertex_list[face_vertices[1].v - 1], vertex_list[face_vertices[2].v - 1], mat);
                    triangles.push_back(tri);
                }
            } else if (face_vertices.size() == 1) {
                int u, v, w;
                ss >> u >> v >> w;
                auto tri = std::make_shared<triangle>(vertex_list[u - 1], vertex_list[v - 1], vertex_list[w - 1], mat);
                triangles.push_back(tri);
            } else if (face_vertices.size() == 4) {
                for (size_t i = 1; i < face_vertices.size() - 1; ++i) {
                    auto tri1 = std::make_shared<triangle>(vertex_list[face_vertices[0].v - 1], vertex_list[face_vertices[1].v - 1], vertex_list[face_vertices[2].v - 1], mat);
                    triangles.push_back(tri1);
                    auto tri2 = std::make_shared<triangle>(vertex_list[face_vertices[2].v - 1], vertex_list[face_vertices[3].v - 1], vertex_list[face_vertices[0].v - 1], mat);
                    triangles.push_back(tri2);
                }
            }
        } 
    }
    return triangles;
}


