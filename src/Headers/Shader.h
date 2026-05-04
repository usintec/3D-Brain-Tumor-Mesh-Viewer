#pragma once
#include <string>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();

    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);

    void setMat4(const std::string& name, const float* value);
    void setVec3(const std::string& name, float x, float y, float z);
};