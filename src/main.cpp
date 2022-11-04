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
Camera *camera;
glm::mat4 projection_matrix(1.0f);
float model_rotation = 0.0f;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
float mouse_last_x = SCREEN_WIDTH / 2.0f;
float mouse_last_y = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//imgui state
bool run_animation = true;
bool capture_mouse = false;


static void GLAPIENTRY debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        return;
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             type, severity, message);
}
void init()
{
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));

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
    woodTexture = new Texture("texture/container.jpg");

    // setup material shader
    // setup light uniform
    glm::vec3 lightColor = glm::vec3(1.0f);

    // setup material shader
    // setup light uniform
    materialShader->use();
    materialShader->setVec3("light.position", 6.0f, 5.0f, 10.0f);
    materialShader->setVec3("light.ambient", lightColor * glm::vec3(0.2f));
    materialShader->setVec3("light.diffuse", lightColor * glm::vec3(0.7f));
    materialShader->setVec3("light.specular", lightColor * glm::vec3(1.0f));

    // setup material uniform
    materialShader->setVec3("material.ambient", 0.95, 0.88, 0.325);
    materialShader->setVec3("material.diffuse", 0.95, 0.88, 0.325);
    materialShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    materialShader->setFloat("material.shininess", 32.0f);


    // setup texture shader
    // setup light uniform
    textureShader->use();
    textureShader->setVec3("light.position", 6.0f, 5.0f, 10.0f);
    textureShader->setVec3("light.ambient", lightColor * glm::vec3(0.2f));
    textureShader->setVec3("light.diffuse", lightColor * glm::vec3(0.6f));
    textureShader->setVec3("light.specular", lightColor * glm::vec3(0.5f));

    // setup material uniform
    textureShader->setVec3("material.ambient", glm::vec3(1.0f));
    textureShader->setVec3("material.diffuse", glm::vec3(1.0f));
    textureShader->setVec3("material.specular", glm::vec3(1.0f));
    textureShader->setFloat("material.shininess", 128.0f);


    textureShader->setInt("textureMap", 0);
    woodTexture->bind(0);

    // setup scene
    scene = new Scene();
    // TODO
    scene->addNodes({
        Scene::SceneNode(cube, textureShader, woodTexture, -1, // body id 0
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.8f, 0.8f, 0.4f)),
        Scene::SceneNode(sphere, materialShader, nullptr, 0, // right hand joint id 1
                         glm::vec3(0.46f, 0.3f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 6.0f),
                         glm::vec3(0.25f, 0.25f, 0.25f)),
        Scene::SceneNode(cube, materialShader, nullptr, 1, // right hand top id 2
                         glm::vec3(0.03f, -0.2f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.13f, 0.5f, 0.13f)),
        Scene::SceneNode(sphere, materialShader, nullptr, 2, // right hand joint id 3
                         glm::vec3(0.0f, -0.22f, 0.0f),
                         glm::vec3(-30.0f, 0.0f, -3.0f),
                         glm::vec3(0.22f, 0.22f, 0.22f)),
        Scene::SceneNode(cube, materialShader, nullptr, 3, // right hand bottom id 4
                         glm::vec3(0.0f, -0.2f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.13f, 0.5f, 0.13f)),
        Scene::SceneNode(sphere, materialShader, nullptr, 0, // right hand joint id 5
                         glm::vec3(-0.46f, 0.3f, 0.0f),
                         glm::vec3(0.0f, 0.0f, -6.0f),
                         glm::vec3(0.25f, 0.25f, 0.25f)),
        Scene::SceneNode(cube, materialShader, nullptr, 5, // right hand top id 6
                         glm::vec3(-0.03f, -0.2f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.13f, 0.5f, 0.13f)),
        Scene::SceneNode(sphere, materialShader, nullptr, 6, // right hand joint id 7
                         glm::vec3(0.0f, -0.22f, 0.0f),
                         glm::vec3(-30.0f, 0.0f, 3.0f),
                         glm::vec3(0.22f, 0.22f, 0.22f)),
        Scene::SceneNode(cube, materialShader, nullptr, 7, // right hand bottom id 8
                         glm::vec3(0.0f, -0.2f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.13f, 0.5f, 0.13f)),
//        Scene::SceneNode(cube, materialShader, nullptr, 0, // left hand top id 3
//                         glm::vec3(-0.55f, 0.15f, 0.0f),
//                         glm::vec3(0.0f, 0.0f, -6.0f),
//                         glm::vec3(0.15f, 0.4f, 0.15f)),
//        Scene::SceneNode(cube, materialShader, nullptr, 3, // left hand bottom id 4
//                         glm::vec3(0.0f, -0.45f, 0.1f),
//                         glm::vec3(-30.0f, 0.0f, 0.0f),
//                         glm::vec3(0.15f, 0.4f, 0.15f)),
//        Scene::SceneNode(cube, materialShader, nullptr, 0, // right leg top id 5
//                         glm::vec3(0.2f, -0.55f, 0.0f),
//                         glm::vec3(0.0f, 0.0f, 0.0f),
//                         glm::vec3(0.15f, 0.3f, 0.15f)),
//        Scene::SceneNode(cube, materialShader, nullptr, 5, // right leg bottom id 6
//                         glm::vec3(0.0f, -0.4f, 0.0f),
//                         glm::vec3(0.0f, 0.0f, 0.0f),
//                         glm::vec3(0.15f, 0.3f, 0.15f)),
//        Scene::SceneNode(cylinder, materialShader, nullptr, 0, // left leg top id 7
//                         glm::vec3(-0.2f, -0.55f, 0.0f),
//                         glm::vec3(0.0f, 0.0f, 0.0f),
//                         glm::vec3(0.15f, 0.15f, 0.15f)),
//        Scene::SceneNode(cylinder, materialShader, nullptr, 7, // left leg bottom id 8
//                         glm::vec3(0.0f, -0.4f, 0.0f),
//                         glm::vec3(0.0f, 0.0f, 0.0f),
//                         glm::vec3(0.15f, 0.15f, 0.15f)),
        Scene::SceneNode(sphere, materialShader, nullptr, 0, // head id 9
                         glm::vec3(0.0f, 0.6f, 0.0f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.5f, 0.5f, 0.5f)),
        Scene::SceneNode(sphere, materialShader, nullptr, 9, // head id 10
                         glm::vec3(0.1f, 0.08f, 0.21f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.05f, 0.07f, 0.05f)),
        Scene::SceneNode(sphere, materialShader, nullptr, 9, // head id 10
                         glm::vec3(-0.1f, 0.08f, 0.21f),
                         glm::vec3(0.0f, 0.0f, 0.0f),
                         glm::vec3(0.05f, 0.07f, 0.05f)),
    });
}
void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // projection & view matrix
    materialShader->use();
    materialShader->setVec3("cameraPosition", camera->position);
    materialShader->setMat4("projection", projection_matrix);
    materialShader->setMat4("view", camera->getViewMatrix());

    textureShader->use();
    textureShader->setVec3("cameraPosition", camera->position);
    textureShader->setMat4("projection", projection_matrix);
    textureShader->setMat4("view", camera->getViewMatrix());

    scene->animate(deltaTime * 1000);
    scene->draw();

}
void toggle_mouse(GLFWwindow* window){
    if (!capture_mouse){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        firstMouse = true;
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    capture_mouse = !capture_mouse;
}
// whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    model_rotation = float(int(model_rotation + 2.0f * y_offset + 360) % 360);
    scene->updateSceneRotation(glm::vec3(0.0f, model_rotation, 0.0f));
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
void mouse_callback(GLFWwindow* window, double x_pos_in, double y_pos_in)
{
    if (!capture_mouse)
        return;
    auto x_pos = static_cast<float>(x_pos_in);
    auto y_pos = static_cast<float>(y_pos_in);

    if (firstMouse)
    {
        mouse_last_x = x_pos;
        mouse_last_y = y_pos;
        firstMouse = false;
    }

    float x_offset = x_pos - mouse_last_x;
    float y_offset = mouse_last_y - y_pos;

    mouse_last_x = x_pos;
    mouse_last_y = y_pos;

    camera->processMouseMovement(x_offset, y_offset);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (action == GLFW_PRESS && key == GLFW_KEY_E){
        if (capture_mouse)
            toggle_mouse(window);
    }
}
void mouse_button_callback(GLFWwindow* window)
{
    toggle_mouse(window);
}
void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(Camera_Movement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera->processKeyboard(Camera_Movement::DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->processKeyboard(Camera_Movement::UP, deltaTime);
    // TODO ijkl to move model
}
void prepare_imgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // right click context menu
    if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && ImGui::IsMouseClicked(1))
        ImGui::OpenPopup("contextmenu");
    if (ImGui::BeginPopup("contextmenu"))
    {
        if (run_animation){
            if(ImGui::Button("Stop animation"))
                run_animation = !run_animation;
        }else{
            if(ImGui::Button("Run animation"))
                run_animation = !run_animation;
        }

        ImGui::Text("Hello");

        if (ImGui::Button("OK")) ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    {
        ImGui::Begin("Info");                          // Create a window called "Hello, world!" and append into it.

        if (ImGui::SliderFloat("model rotation", &model_rotation, 0.0f, 360.0f))
            scene->updateSceneRotation(glm::vec3(0.0f, model_rotation, 0.0f));

        ImGui::Text("Application %.1f FPS", ImGui::GetIO().Framerate);
        ImGui::End();
    }

}
int main(int argc, char *argv[])
{
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graphics programming assignment 1", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    // setup callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

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

    // setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // setup viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);
    projection_matrix = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

    init();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // calculate frame time
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_input(window);

        prepare_imgui();
        // left click to lock screen for camera movement
        if (!io.WantCaptureMouse && ImGui::IsMouseClicked(0)) {
            mouse_button_callback(window);
        }

        // Render
        ImGui::Render();
        draw();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}