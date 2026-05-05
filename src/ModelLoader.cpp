#include "ModelLoader.h"
#include <fstream>
#include <sstream>
#include <cmath>

void computeNormals(std::vector<Vertex>& vertices) {

    // initialize normals
    for (auto& v : vertices) {
        v.normal[0] = 0.0f;
        v.normal[1] = 0.0f;
        v.normal[2] = 0.0f;
    }

    // process triangles (3 vertices per face)
    for (size_t i = 0; i < vertices.size(); i += 3) {

        Vertex& v0 = vertices[i];
        Vertex& v1 = vertices[i + 1];
        Vertex& v2 = vertices[i + 2];

        // edges
        float e1[3] = {
            v1.position[0] - v0.position[0],
            v1.position[1] - v0.position[1],
            v1.position[2] - v0.position[2]
        };

        float e2[3] = {
            v2.position[0] - v0.position[0],
            v2.position[1] - v0.position[1],
            v2.position[2] - v0.position[2]
        };

        // cross product (normal)
        float n[3] = {
            e1[1] * e2[2] - e1[2] * e2[1],
            e1[2] * e2[0] - e1[0] * e2[2],
            e1[0] * e2[1] - e1[1] * e2[0]
        };

        // accumulate
        for (int j = 0; j < 3; j++) {
            vertices[i].normal[j] += n[j];
            vertices[i + 1].normal[j] += n[j];
            vertices[i + 2].normal[j] += n[j];
        }
    }

    // normalize
    for (auto& v : vertices) {
        float length = sqrt(
            v.normal[0] * v.normal[0] +
            v.normal[1] * v.normal[1] +
            v.normal[2] * v.normal[2]
        );

        if (length > 0.0f) {
            v.normal[0] /= length;
            v.normal[1] /= length;
            v.normal[2] /= length;
        }
    }
}

Mesh ModelLoader::loadOBJ(const std::string &path)
{
    std::vector<float> positions;
    std::vector<Vertex> vertices;

    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v")
        {
            float x, y, z;
            ss >> x >> y >> z;
            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);
        }

        if (type == "f")
        {
            std::vector<int> indices;
            int idx;
            while (ss >> idx)
            {
                indices.push_back(idx);
            }

            // Triangulate the face (assuming convex polygon)
            if (indices.size() >= 3)
            {
                for (size_t i = 1; i < indices.size() - 1; ++i)
                {
                    for (int j : {0, (int)i, (int)i + 1})
                    {
                        int vidx = indices[j] - 1;
                        Vertex v{};
                        v.position[0] = positions[vidx * 3 + 0];
                        v.position[1] = positions[vidx * 3 + 1];
                        v.position[2] = positions[vidx * 3 + 2];

                        // simple normal (placeholder)
                        v.normal[0] = 0.0f;
                        v.normal[1] = 1.0f;
                        v.normal[2] = 0.0f;

                        vertices.push_back(v);
                    }
                }
            }
        }
    }
    // AFTER building vertices:
    computeNormals(vertices);
    return Mesh(vertices);
}