#include "ModelLoader.h"
#include <fstream>
#include <sstream>
#include <cmath>

Mesh ModelLoader::loadOBJ(const std::string &path)
{
    std::vector<float> positions;
    std::vector<int> faceIndices; // Store face indices for normal computation

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
                        faceIndices.push_back(indices[j] - 1); // OBJ indices are 1-based
                    }
                }
            }
        }
    }

    // Now create vertices with smooth normals
    std::vector<float> normals(positions.size(), 0.0f);

    // Compute face normals and accumulate at each vertex position
    for (size_t i = 0; i < faceIndices.size(); i += 3)
    {
        int i0 = faceIndices[i];
        int i1 = faceIndices[i + 1];
        int i2 = faceIndices[i + 2];

        float v0[3] = {positions[i0 * 3], positions[i0 * 3 + 1], positions[i0 * 3 + 2]};
        float v1[3] = {positions[i1 * 3], positions[i1 * 3 + 1], positions[i1 * 3 + 2]};
        float v2[3] = {positions[i2 * 3], positions[i2 * 3 + 1], positions[i2 * 3 + 2]};

        // Compute cross product (face normal)
        float e1[3] = {v1[0] - v0[0], v1[1] - v0[1], v1[2] - v0[2]};
        float e2[3] = {v2[0] - v0[0], v2[1] - v0[1], v2[2] - v0[2]};

        float n[3] = {
            e1[1] * e2[2] - e1[2] * e2[1],
            e1[2] * e2[0] - e1[0] * e2[2],
            e1[0] * e2[1] - e1[1] * e2[0]};

        // Accumulate to all three vertices
        for (int j = 0; j < 3; j++)
        {
            normals[i0 * 3 + j] += n[j];
            normals[i1 * 3 + j] += n[j];
            normals[i2 * 3 + j] += n[j];
        }
    }

    // Normalize all normals
    for (size_t i = 0; i < normals.size(); i += 3)
    {
        float len = sqrt(normals[i] * normals[i] + normals[i + 1] * normals[i + 1] + normals[i + 2] * normals[i + 2]);
        if (len > 0.0f)
        {
            normals[i] /= len;
            normals[i + 1] /= len;
            normals[i + 2] /= len;
        }
    }

    // Create final vertex list
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < faceIndices.size(); i++)
    {
        int idx = faceIndices[i];
        Vertex v{};
        v.position[0] = positions[idx * 3 + 0];
        v.position[1] = positions[idx * 3 + 1];
        v.position[2] = positions[idx * 3 + 2];
        v.normal[0] = normals[idx * 3 + 0];
        v.normal[1] = normals[idx * 3 + 1];
        v.normal[2] = normals[idx * 3 + 2];
        vertices.push_back(v);
    }

    return Mesh(vertices);
}