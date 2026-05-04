#pragma once

class Camera {
public:
    float position[3];

    Camera();

    // Returns raw float pointers (column-major matrices)
    const float* getViewMatrix();
    const float* getProjectionMatrix(float aspect);
};