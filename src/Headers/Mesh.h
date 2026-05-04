#pragma once
#include <vector>

struct Vertex {
    float position[3];
    float normal[3];
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    unsigned int VAO, VBO;

    Mesh(std::vector<Vertex> verts);
    void draw();
};