#include <fstream>
#include <sstream>
#include "core/vec3.h"
#include "geometry/triangle.h"
#include "materials/lambertian.h"
#include "core/color.h"
inline std::vector<std::shared_ptr<hittable>> read_obj(std::ifstream& infile, std::shared_ptr <material> mat){
    std::vector<point3> vertex_list;
    std::vector<std::shared_ptr<hittable>> triangles;
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
        } else if (tag == "f") { // face
            int u, v, w;
            ss >> u >> v >> w;
            auto tri = std::make_shared<triangle>(vertex_list[u - 1], vertex_list[v - 1], vertex_list[w - 1], mat);
            triangles.push_back(tri);
        } 
    }
    return triangles;
}


