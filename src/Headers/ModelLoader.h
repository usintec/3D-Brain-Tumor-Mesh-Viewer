#pragma once
#include "Mesh.h"
#include <string>

class ModelLoader {
public:
    static Mesh loadOBJ(const std::string& path);
};