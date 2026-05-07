#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <algorithm>

#include "Headers/Shader.h"
#include "Headers/Mesh.h"
#include "Headers/ModelLoader.h"
#include "Headers/Camera.h"

struct TumorMesh
{
    Mesh mesh;
    const char *name;
};

int main()
{
    glfwInit();

    GLFWwindow *window = glfwCreateWindow(800, 600, "Medical Viewer", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glViewport(0, 0, 800, 600);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::string filePath = __FILE__;
    size_t srcPos = filePath.find("\\src");
    std::string projectRoot = filePath.substr(0, srcPos);
    std::string vertPath = projectRoot + "\\shaders\\Mesh.vert";
    std::string fragPath = projectRoot + "\\shaders\\Mesh.frag";

    // Load all tumor meshes in render order: WT => TC => ET
    std::vector<TumorMesh> tumors;

    // Whole tumor reference (tumor_mesh.obj)
    tumors.push_back({ModelLoader::loadOBJ((projectRoot + "\\assets\\tumor_mesh.obj").c_str()),
                      "tumor_mesh.obj"});

    // Whole tumor (wt)
    tumors.push_back({ModelLoader::loadOBJ((projectRoot + "\\assets\\wt.obj").c_str()),
                      "wt.obj"});

    // Tumor core (tc)
    tumors.push_back({ModelLoader::loadOBJ((projectRoot + "\\assets\\tc.obj").c_str()),
                      "tc.obj"});

    // Enhanced tumor (et)
    tumors.push_back({ModelLoader::loadOBJ((projectRoot + "\\assets\\et.obj").c_str()),
                      "et.obj"});

    Shader shader(vertPath.c_str(), fragPath.c_str());
    Camera camera;

    camera.position[0] = 0.0f;
    camera.position[1] = 0.0f;
    camera.position[2] = 10.0f;

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

        float s = 0.02f;
        // model rotation
        float angle = glfwGetTime();

        float model[16] = {
            s * cos(angle), 0, s * sin(angle), 0,
            0, s, 0, 0,
            -s * sin(angle), 0, s * cos(angle), 0,
            0, 0, 0, 1};

        shader.setMat4("model", model);

        // Transform light position to follow the rotating object
        float lightPosWorld[3] = {2.0f, 2.0f, 2.0f}; // Fixed light in world space
        float lightPosModel[3] = {
            model[0] * lightPosWorld[0] + model[4] * lightPosWorld[1] + model[8] * lightPosWorld[2] + model[12],
            model[1] * lightPosWorld[0] + model[5] * lightPosWorld[1] + model[9] * lightPosWorld[2] + model[13],
            model[2] * lightPosWorld[0] + model[6] * lightPosWorld[1] + model[10] * lightPosWorld[2] + model[14]};
            
        shader.setVec3("lightPos", lightPosModel[0], lightPosModel[1], lightPosModel[2]);
        
        shader.setVec3("viewPos",
                       camera.position[0],
                       camera.position[1],
                       camera.position[2]);

        // Draw each tumor mesh in correct layering order: TC (back) -> ET (front) -> WT -> background
        glDepthMask(GL_FALSE); // Disable depth writing for transparent objects
        std::vector<std::string> renderOrder = { "et.obj", "tc.obj", "wt.obj", "tumor_mesh.obj"};
        for (const auto &meshName : renderOrder)
        {
            auto it = std::find_if(tumors.begin(), tumors.end(),
                                   [&meshName](const TumorMesh &t)
                                   { return t.name == meshName; });
            if (it != tumors.end())
            {
                int tumorClass = 0; // default tumor_mesh (grey background)
                if (it->name == "wt.obj")
                    tumorClass = 3; // WT (blue)
                else if (it->name == "tc.obj")
                    tumorClass = 2; // TC (orange)
                else if (it->name == "et.obj")
                    tumorClass = 1; // ET (red)

                shader.setInt("tumorClass", tumorClass);
                it->mesh.draw();
            }
        }
        glDepthMask(GL_TRUE); // Re-enable depth writing

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}