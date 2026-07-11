#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Shader.h"
#include "Headers/Mesh.h"
#include "Headers/ModelLoader.h"
#include "Headers/Camera.h"
#include "Headers/Evaluation.h"

struct TumorMesh
{
    Mesh mesh;
    const char* name;
};

int main()
{
    //-----------------------------------
    // GLFW / OpenGL Init
    //-----------------------------------

    glfwInit();

    GLFWwindow* window =
        glfwCreateWindow(
            800,
            600,
            "Medical Viewer",
            NULL,
            NULL);

    glfwMakeContextCurrent(window);

    gladLoadGLLoader(
        (GLADloadproc)glfwGetProcAddress);

    glViewport(0, 0, 800, 600);

    glEnable(GL_BLEND);
    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //-----------------------------------
    // Shader Paths
    //-----------------------------------

    std::string filePath = __FILE__;

    size_t srcPos =
        filePath.find("\\src");

    std::string projectRoot =
        filePath.substr(0, srcPos);

    std::string vertPath =
        projectRoot +
        "\\shaders\\Mesh.vert";

    std::string fragPath =
        projectRoot +
        "\\shaders\\Mesh.frag";

    //-----------------------------------
    // Load Meshes
    //-----------------------------------

    std::vector<TumorMesh> tumors;

    // Whole tumor reference mesh
    tumors.push_back({
        ModelLoader::loadOBJ(
            (projectRoot +
             "\\assets\\tumor_mesh.obj")
                .c_str()),
        "tumor_mesh.obj" });

    // WT
    tumors.push_back({
        ModelLoader::loadOBJ(
            (projectRoot +
             "\\assets\\wt.obj")
                .c_str()),
        "wt.obj" });

    // TC
    tumors.push_back({
        ModelLoader::loadOBJ(
            (projectRoot +
             "\\assets\\tc.obj")
                .c_str()),
        "tc.obj" });

    // ET
    tumors.push_back({
        ModelLoader::loadOBJ(
            (projectRoot +
             "\\assets\\et.obj")
                .c_str()),
        "et.obj" });
        
    //-----------------------------------
    // Shader
    //-----------------------------------

    Shader shader(
        vertPath.c_str(),
        fragPath.c_str());

    //-----------------------------------
    // Camera
    //-----------------------------------

    Camera camera;

    camera.position[0] = 0.0f;
    camera.position[1] = 0.0f;
    camera.position[2] = 10.0f;

    //-----------------------------------
    // Model Controls
    //-----------------------------------

    float modelRotateX = 0.0f;
    float modelRotateY = 0.0f;

    float modelPanX = 0.0f;
    float modelPanY = 0.0f;

    float modelZoom = 1.0f;

    //-----------------------------------
    // Timing
    //-----------------------------------

    float lastFrame =
        glfwGetTime();

    //-----------------------------------
    // Render Loop
    //-----------------------------------

    while (!glfwWindowShouldClose(window))
    {
        //-----------------------------------
        // Delta Time
        //-----------------------------------

        float currentFrame =
            glfwGetTime();

        float deltaTime =
            currentFrame - lastFrame;

        lastFrame = currentFrame;

        //-----------------------------------
        // Input
        //-----------------------------------

        float rotateSpeed = 1.5f;
        float panSpeed = 2.0f;
        float zoomSpeed = 1.0f;

        // Rotate
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            modelRotateY -= rotateSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            modelRotateY += rotateSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            modelRotateX -= rotateSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            modelRotateX += rotateSpeed * deltaTime;

        // Pan
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            modelPanX -= panSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            modelPanX += panSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            modelPanY += panSpeed * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            modelPanY -= panSpeed * deltaTime;

        // Zoom
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            modelZoom =
            fmax(
                0.1f,
                modelZoom -
                zoomSpeed * deltaTime);

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            modelZoom +=
            zoomSpeed * deltaTime;

        // ----------------------------------
        // FPS Evaluation
        // ----------------------------------

        float fps =
            Evaluation::computeFPS(
                deltaTime
            );

        std::cout
            << "FPS: "
            << fps
            << std::endl;

        // ----------------------------------
       // Gradient Evaluation
       // ----------------------------------
        float grad =
        Evaluation::gradientMagnitude(
            4.2f,
            3.1f
        );

        std::cout
            << "Gradient: "
            << grad
            << std::endl;

        // ----------------------------------
       // Edge Visibility Evaluation
       // ----------------------------------
        float edge =
            Evaluation::edgeVisibility(
                0.3f
            );

        std::cout
            << "Edge Visibility: "
            << edge
            << std::endl;

        // ----------------------------------
       // Silhouette Evaluation
       // ----------------------------------
        
        std::vector<float> silhouetteValues =
        {
            0.72f,
            0.81f,
            0.78f,
            0.84f
        };

        float silhouette =
            Evaluation::silhouetteVisibility(
                silhouetteValues
            );

        std::cout
            << "Silhouette Visibility: "
            << silhouette
            << std::endl;
        //-----------------------------------
        // Clear Screen
        //-----------------------------------

        glClearColor(
            0.01f, 0.01f, 0.015f,
            1.0f);

        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);

        //-----------------------------------
        // Use Shader
        //-----------------------------------

        shader.use();
        shader.setFloat(
            "time",
            glfwGetTime()
        );

        //-----------------------------------
        // Viewport
        //-----------------------------------

        int width, height;

        glfwGetFramebufferSize(
            window,
            &width,
            &height);

        float aspect =
            (float)width /
            (float)height;

        //-----------------------------------
        // Camera Matrices
        //-----------------------------------

        shader.setMat4(
            "view",
            camera.getViewMatrix());

        shader.setMat4(
            "projection",
            camera.getProjectionMatrix(aspect));

        //-----------------------------------
        // Proper Model Matrix
        //-----------------------------------

        glm::mat4 model =
            glm::mat4(1.0f);

        // Translation
        model =
            glm::translate(
                model,
                glm::vec3(
                    modelPanX,
                    modelPanY,
                    0.0f));

        // Rotation X
        model =
            glm::rotate(
                model,
                modelRotateX,
                glm::vec3(
                    1.0f,
                    0.0f,
                    0.0f));

        // Rotation Y
        model =
            glm::rotate(
                model,
                modelRotateY,
                glm::vec3(
                    0.0f,
                    1.0f,
                    0.0f));

        // Scale
        model =
            glm::scale(
                model,
                glm::vec3(
                    0.02f * modelZoom));

        //-----------------------------------
        // Send Model Matrix
        //-----------------------------------

        shader.setMat4(
            "model",
            glm::value_ptr(model));

        //-----------------------------------
        // Fixed Light Position
        //-----------------------------------

        shader.setVec3(
            "lightPos",
            2.0f,
            2.0f,
            2.0f);

        //-----------------------------------
        // Camera Position
        //-----------------------------------

        shader.setVec3(
            "viewPos",
            camera.position[0],
            camera.position[1],
            camera.position[2]);

        //-----------------------------------
        // Render Order
        //-----------------------------------

        std::vector<std::string> renderOrder =
        {
            "tumor_mesh.obj",
            "wt.obj",
            "tc.obj",
            "et.obj" };

        //-----------------------------------
        // Transparent Rendering
        //-----------------------------------

        glDepthMask(GL_FALSE);

        for (const auto& meshName : renderOrder)
        {
            auto it =
                std::find_if(
                    tumors.begin(),
                    tumors.end(),
                    [&meshName](const TumorMesh& t)
                    {
                        return t.name == meshName;
                    });

            if (it != tumors.end())
            {
                int tumorClass = 0;

                // ET
                if (it->name == std::string("et.obj"))
                    tumorClass = 1;

                // TC
                else if (it->name == std::string("tc.obj"))
                    tumorClass = 2;

                // WT
                else if (it->name == std::string("wt.obj"))
                    tumorClass = 3;

                shader.setInt(
                    "tumorClass",
                    tumorClass);

                it->mesh.draw();
            }
        }

        glDepthMask(GL_TRUE);

        //-----------------------------------
        // Present Frame
        //-----------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //-----------------------------------
    // Cleanup
    //-----------------------------------

    glfwTerminate();

    return 0;
}