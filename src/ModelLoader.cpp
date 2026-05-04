#include "ModelLoader.h"
#include <fstream>
#include <sstream>

Mesh ModelLoader::loadOBJ(const std::string& path) {
    std::vector<float> positions;
    std::vector<Vertex> vertices;

    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);
        }

        if (type == "f") {
            int a, b, c;
            ss >> a >> b >> c;

            for (int idx : { a, b, c }) {
                Vertex v{};
                v.position[0] = positions[(idx - 1) * 3 + 0];
                v.position[1] = positions[(idx - 1) * 3 + 1];
                v.position[2] = positions[(idx - 1) * 3 + 2];

                // simple normal (placeholder)
                v.normal[0] = 0.0f;
                v.normal[1] = 1.0f;
                v.normal[2] = 0.0f;

                vertices.push_back(v);
            }
        }
    }

    return Mesh(vertices);
}