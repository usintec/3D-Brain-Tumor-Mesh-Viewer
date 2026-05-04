#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "Headers/Shader.h"
#include "Headers/Mesh.h"
#include "Headers/ModelLoader.h"
#include "Headers/Camera.h"

int main()
{
    glfwInit();

    GLFWwindow *window = glfwCreateWindow(800, 600, "Medical Viewer", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glViewport(0, 0, 800, 600);

    std::string filePath = __FILE__;
    size_t srcPos = filePath.find("\\src");
    std::string projectRoot = filePath.substr(0, srcPos);
    std::string modelPath = projectRoot + "\\assets\\tum.obj";
    std::string vertPath = projectRoot + "\\shaders\\Mesh.vert";
    std::string fragPath = projectRoot + "\\shaders\\Mesh.frag";

    Mesh mesh = ModelLoader::loadOBJ(modelPath);
    Shader shader(vertPath.c_str(), fragPath.c_str());
    Camera camera;

    while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        shader.use();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        float aspect = (float)width / (float)height;

        // set matrices (you already have camera logic)
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", camera.getProjectionMatrix(aspect));

        // model rotation
        float angle = glfwGetTime();

        float model[16] = {
            cos(angle), 0, sin(angle), 0,
            0, 1, 0, 0,
            -sin(angle), 0, cos(angle), 0,
            0, 0, 0, 1};

        shader.setMat4("model", model);

        shader.setVec3("lightPos", 2.0f, 2.0f, 2.0f);
        shader.setVec3("viewPos",
                       camera.position[0],
                       camera.position[1],
                       camera.position[2]);

        mesh.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}