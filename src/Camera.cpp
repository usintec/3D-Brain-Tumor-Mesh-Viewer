#include "Camera.h"
#include <cmath>

// Simple identity matrix
static float identity[16] = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1
};

// Simple projection matrix (perspective)
static float projection[16];

Camera::Camera() {
    position[0] = 0.0f;
    position[1] = 0.0f;
    position[2] = 3.0f;
}

const float* Camera::getViewMatrix() {
    // Very simple camera (no rotation yet)
    static float view[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    // translate backwards
    view[14] = -position[2];

    return view;
}

const float* Camera::getProjectionMatrix(float aspect) {
    float fov = 45.0f * 3.14159f / 180.0f;
    float near = 0.1f;
    float far = 100.0f;

    float f = 1.0f / tan(fov / 2.0f);

    for (int i = 0; i < 16; i++) projection[i] = 0.0f;

    projection[0] = f / aspect;
    projection[5] = f;
    projection[10] = (far + near) / (near - far);
    projection[11] = -1.0f;
    projection[14] = (2 * far * near) / (near - far);

    return projection;
}