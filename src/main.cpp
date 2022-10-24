#include "Common.h"


Shader *materialShader;
Shader *textureShader;
Model* capsule;
Model* cube;
Model* cylinder;
Model* plane;
Model* sphere;
Texture* woodTexture;
Scene *scene;
glm::mat4 projection_matrix(1.0f);
float model_rotation = 0.0f;

void init()
{
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // Load shaders
    materialShader = new Shader("shader/material.vs.glsl", "shader/material.fs.glsl");
    textureShader = new Shader("shader/texture.vs.glsl", "shader/texture.fs.glsl");

    // Load models
    capsule = new Model("model/Capsule.obj");
    cube = new Model("model/Cube.obj");
    cylinder = new Model("model/Cylinder.obj");
    plane = new Model("model/Plane.obj");
    sphere = new Model("model/Sphere.obj");

    // Load textures
    woodTexture = new Texture("texture/wood.jpg");

    // setup material shader
    // setup light uniform
    glm::vec3 lightColor = glm::vec3(1.0f);


    materialShader->use();
    materialShader->setVec3("light.position", 3.0f, 5.0f, 10.0f);
    materialShader->setVec3("light.ambient", lightColor * glm::vec3(0.2f));
    materialShader->setVec3("light.diffuse", lightColor * glm::vec3(0.85f));
    materialShader->setVec3("light.specular", lightColor * glm::vec3(1.0f));

    // setup material uniform
    materialShader->setVec3("material.ambient", 0.24, 0.863, 0.518);
    materialShader->setVec3("material.diffuse", 0.24, 0.863, 0.518);
    materialShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    materialShader->setFloat("material.shininess", 32.0f);

    materialShader->setVec3("cameraPosition", 0.0f, 0.0f, 6.0f);

    // setup texture shader
    // setup light uniform
    textureShader->use();
    textureShader->setVec3("light.position", 3.0f, 5.0f, 10.0f);
    textureShader->setVec3("light.ambient", lightColor * glm::vec3(0.2f));
    textureShader->setVec3("light.diffuse", lightColor * glm::vec3(0.8f));
    textureShader->setVec3("light.specular", lightColor * glm::vec3(1.0f));

    // setup material uniform
    textureShader->setVec3("material.ambient", glm::vec3(1.0f));
    textureShader->setVec3("material.diffuse", glm::vec3(1.0f));
    textureShader->setVec3("material.specular", glm::vec3(0.5f));
    textureShader->setFloat("material.shininess", 64.0f);


    textureShader->setVec3("cameraPosition", 0.0f, 0.0f, 6.0f);

    textureShader->setInt("textureMap", 0);
    woodTexture->bind(0);

    // setup scene
    scene = new Scene();
    scene->addNodes({
        Scene::SceneNode(cube, textureShader, woodTexture, -1, // body id 0
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.8f, 0.8f, 0.4f)),
        Scene::SceneNode(cube, materialShader, nullptr, 0, // right hand top id 1
                         glm::vec3(0.55f, 0.15f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 6.0f),
                         glm::vec3(0.15f, 0.4f, 0.15f)),
        Scene::SceneNode(cube, materialShader, nullptr, 1, // right hand bottom id 2
                         glm::vec3(0.0f, -0.45f, 0.1f),
                         glm::vec3(-30.0f, 0.0f, 0.0f),
                         glm::vec3(0.15f, 0.4f, 0.15f)),
        Scene::SceneNode(cube, materialShader, nullptr, 0, // left hand top id 3
                         glm::vec3(-0.55f, 0.15f, 0.0f),
                         glm::vec3(0.0f, 0.0f, -6.0f),
                         glm::vec3(0.15f, 0.4f, 0.15f)),
        Scene::SceneNode(cube, materialShader, nullptr, 3, // left hand bottom id 4
                         glm::vec3(0.0f, -0.45f, 0.1f),
                         glm::vec3(-30.0f, 0.0f, 0.0f),
                         glm::vec3(0.15f, 0.4f, 0.15f)),
        Scene::SceneNode(cube, materialShader, nullptr, 0, // right leg top id 5
                         glm::vec3(0.2f, -0.55f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.15f, 0.3f, 0.15f)),
        Scene::SceneNode(cube, materialShader, nullptr, 5, // right leg bottom id 6
                         glm::vec3(0.0f, -0.4f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.15f, 0.3f, 0.15f)),
        Scene::SceneNode(cylinder, materialShader, nullptr, 0, // left leg top id 7
                         glm::vec3(-0.2f, -0.55f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.15f, 0.15f, 0.15f)),
        Scene::SceneNode(cylinder, materialShader, nullptr, 7, // left leg bottom id 8
                         glm::vec3(0.0f, -0.4f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.15f, 0.15f, 0.15f)),
        Scene::SceneNode(sphere, materialShader, nullptr, 0, // head id 9
                         glm::vec3(0.0f, 0.6f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.5f, 0.4f, 0.4f))
    });
}

// GLUT callback. Called to draw the scene.
void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // projection & view matrix
    glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, -6.0f));

    materialShader->use();
    materialShader->setMat4("projection", projection_matrix);
    materialShader->setMat4("view", view_matrix);

    textureShader->use();
    textureShader->setMat4("projection", projection_matrix);
    textureShader->setMat4("view", view_matrix);

    scene->draw();

}


// whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    model_rotation = float(int(model_rotation + 2.0f * yoffset) % 360);
    scene->updateRotation(glm::vec3(0.0f, model_rotation, 0.0f));
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    projection_matrix = glm::perspective(glm::radians(45.0f), width / (float) height, 0.1f, 100.0f);

    // Re-render the scene because the current frame was drawn for the old resolution
    draw();
    glfwSwapBuffers(window);
}
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void GLAPIENTRY debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
{
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             type, severity, message);
}
int main(int argc, char *argv[])
{
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(640, 480, "Graphics programming assignment 1", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSwapInterval(1);

    glEnable(GL_DEBUG_OUTPUT);
    if (glDebugMessageCallback)
        glDebugMessageCallback(debugMessageCallback, nullptr);

    printGLContextInfo();
    init();

    while (!glfwWindowShouldClose(window))
    {
        float viewportAspect;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        projection_matrix = glm::perspective(glm::radians(45.0f), width / (float) height, 0.1f, 100.0f);

        draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}