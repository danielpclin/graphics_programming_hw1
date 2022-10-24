#include "Common.h"


Shader *materialShader;
Shader *textureShader;
Model* capsule;
Model* cube;
Model* cylinder;
Model* plane;
Model* sphere;
Texture* treeTexture;
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
    treeTexture = new Texture("texture/test.jpg");

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

    materialShader->setVec3("cameraPosition", 0.0f, 0.0f, 3.0f);

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


    textureShader->setVec3("cameraPosition", 0.0f, 0.0f, 3.0f);

    textureShader->setInt("textureMap", 0);
    treeTexture->bind(0);

    // setup scene
    scene = new Scene();
    // body
    scene->addNode(Scene::SceneNode(capsule, textureShader, treeTexture,
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(0.8f, 0.8f, 0.8f)));
    // right hand
    scene->addNode(Scene::SceneNode(capsule, materialShader, nullptr,
                                    glm::vec3(0.55f, -0.1f, 0.0f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(0.2f, 0.2f, 0.2f)));
    // right hand
    scene->addNode(Scene::SceneNode(capsule, materialShader, nullptr,
                                    glm::vec3(0.55f, 0.1f, 0.0f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(0.2f, 0.2f, 0.2f)));
    // left hand
    scene->addNode(Scene::SceneNode(capsule, materialShader, nullptr,
                                    glm::vec3(-0.55f, -0.08f, 0.0f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(0.2f, 0.2f, 0.2f)));
    // left hand
    scene->addNode(Scene::SceneNode(capsule, materialShader, nullptr,
                                    glm::vec3(-0.55f, 0.08f, 0.0f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(0.2f, 0.2f, 0.2f)));
    // right leg
    scene->addNode(Scene::SceneNode(capsule, materialShader, nullptr,
                                    glm::vec3(0.2f, -0.55f, 0.0f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(0.2f, 0.2f, 0.2f)));
    // left leg
    scene->addNode(Scene::SceneNode(capsule, materialShader, nullptr,
                                    glm::vec3(-0.2f, -0.55f, 0.0f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(0.2f, 0.2f, 0.2f)));
    scene->updateMatrices();
}

// GLUT callback. Called to draw the scene.
void render_callback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // projection & view matrix

    glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, -3.0f));

    materialShader->use();
    materialShader->setMat4("projection", projection_matrix);
    materialShader->setMat4("view", view_matrix);

    textureShader->use();
    textureShader->setMat4("projection", projection_matrix);
    textureShader->setMat4("view", view_matrix);

    scene->draw();

    glutSwapBuffers();
    glutPostRedisplay();

}

void reshape_callback(int width, int height)
{
    glViewport(0, 0, width, height);

    float viewportAspect = (float)width / (float)height;

    projection_matrix = glm::perspective(glm::radians(45.0f), viewportAspect, 0.1f, 100.0f);
}
// whenever the mouse clicks, this callback is called
void mouse_callback(int button, int state, int x, int y)
{

}

// whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(int wheel, int direction, int x, int y)
{
    model_rotation = float(int(model_rotation + 2.0f * direction) % 360);
    scene->updateRotation(glm::vec3(0.0f, model_rotation, 0.0f));
}
void timer_callback(int val)
{
    glutPostRedisplay();
    glutTimerFunc(16, timer_callback, val);
}

static void GLAPIENTRY debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
{
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             type, severity, message);
}

int main(int argc, char *argv[])
{
    // Initialize GLUT and GLEW, then create a window.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Graphics programming assignment 1"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
    glewInit();

    glEnable(GL_DEBUG_OUTPUT);
    if (glDebugMessageCallback)
        glDebugMessageCallback(debugMessageCallback, nullptr);

    printGLContextInfo();
    init();

    // Register GLUT callback functions.
    glutDisplayFunc(render_callback);
    glutReshapeFunc(reshape_callback);
    glutMouseFunc(mouse_callback);
    glutMouseWheelFunc(scroll_callback);
//    glutKeyboardFunc();
    glutTimerFunc(16, timer_callback, 0);

    // Enter main event loop.
    glutMainLoop();

    return 0;
}