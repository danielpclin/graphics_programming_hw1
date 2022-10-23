#include "Common.h"


Model* capsule;
Model* cube;
Model* cylinder;
Model* plane;
Model* sphere;
Shader *materialShader;
Shader *textureShader;
glm::mat4 projection_matrix(1.0f);
float model_rotation = 0.0f;

void init()
{
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    materialShader = new Shader("shader/material.vs.glsl", "shader/material.fs.glsl");
    textureShader = new Shader("shader/texture.vs.glsl", "shader/texture.fs.glsl");
    capsule = new Model("obj/Capsule.obj");
    cube = new Model("obj/Cube.obj");
    cylinder = new Model("obj/Cylinder.obj");
    plane = new Model("obj/Plane.obj");
    sphere = new Model("obj/Sphere.obj");
    materialShader->use();

    // setup light uniform
    glm::vec3 lightColor = glm::vec3(1.0f);

    glm::vec3 ambientWeight = lightColor * glm::vec3(0.2f);
    glm::vec3 diffuseWeight = lightColor * glm::vec3(0.5f);
    glm::vec3 specularWeight = lightColor * glm::vec3(1.0f);

    materialShader->setVec3("light.position", 3.0f, 5.0f, 10.0f);
    materialShader->setVec3("light.ambient", ambientWeight);
    materialShader->setVec3("light.diffuse", diffuseWeight);
    materialShader->setVec3("light.specular", specularWeight);

    // setup material uniform
    materialShader->setVec3("material.ambient", 1.0f, 0.5f, 0.3f);
    materialShader->setVec3("material.diffuse", 1.0f, 0.5f, 0.3f);
    materialShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    materialShader->setFloat("material.shininess", 32.0f);

    materialShader->setVec3("cameraPosition", 0.0f, 0.0f, 3.0f);
}

// GLUT callback. Called to draw the scene.
void render_callback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // projection
    materialShader->setMat4("projection", projection_matrix);

    // view
    glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
//    view_matrix = glm::rotate(view_matrix, glm::radians(model_rotation), glm::vec3(0, 1, 0));

    materialShader->setMat4("view", view_matrix);

    // model 1
    glm::mat4 model_matrix(1.0f);
    model_matrix = glm::rotate(model_matrix, glm::radians(model_rotation), glm::vec3(0, 1, 0));
    model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, glm::radians(60.0f), glm::vec3(1, 0, 0));
    model_matrix = glm::rotate(model_matrix, glm::radians(0.0f), glm::vec3(0, 1, 0));
    model_matrix = glm::rotate(model_matrix, glm::radians(0.0f), glm::vec3(0, 0, 1));
    model_matrix = glm::scale(model_matrix, glm::vec3(0.5f));

    materialShader->setMat4("model", model_matrix);
    cylinder->bind();
    glDrawArrays(GL_TRIANGLES, 0, cylinder->vertexCount);


    glm::mat4 model2_matrix(1.0f);
    model2_matrix = glm::rotate(model2_matrix, glm::radians(model_rotation), glm::vec3(0, 1, 0));
    model2_matrix = glm::translate(model2_matrix, glm::vec3(1.0f, 0.0f, 0.0f));
    model2_matrix = glm::rotate(model2_matrix, glm::radians(0.0f), glm::vec3(1, 0, 0));
    model2_matrix = glm::rotate(model2_matrix, glm::radians(0.0f), glm::vec3(0, 1, 0));
    model2_matrix = glm::rotate(model2_matrix, glm::radians(60.0f), glm::vec3(0, 0, 1));
    model2_matrix = glm::scale(model2_matrix, glm::vec3(0.5f));

    materialShader->setMat4("model", model2_matrix);
    cube->bind();
    glDrawArrays(GL_TRIANGLES, 0, cube->vertexCount);


    glm::mat4 model3_matrix(1.0f);
    model3_matrix = glm::rotate(model3_matrix, glm::radians(model_rotation), glm::vec3(0, 1, 0));
    model3_matrix = glm::translate(model3_matrix, glm::vec3(0.0f, 0.0f, 1.0f));
    model3_matrix = glm::rotate(model3_matrix, glm::radians(0.0f), glm::vec3(1, 0, 0));
    model3_matrix = glm::rotate(model3_matrix, glm::radians(0.0f), glm::vec3(0, 1, 0));
    model3_matrix = glm::rotate(model3_matrix, glm::radians(60.0f), glm::vec3(0, 0, 1));
    model3_matrix = glm::scale(model3_matrix, glm::vec3(0.5f));

    materialShader->setMat4("model", model3_matrix);
    capsule->bind();
    glDrawArrays(GL_TRIANGLES, 0, capsule->vertexCount);


    glutSwapBuffers();
    glutPostRedisplay();

}

void reshape_callback(int width, int height)
{
    glViewport(0, 0, width, height);

    float viewportAspect = (float)width / (float)height;

    projection_matrix = glm::perspective(glm::radians(45.0f), viewportAspect, 0.1f, 100.0f);
}
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(int button, int state, int x, int y)
{

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(int wheel, int direction, int x, int y)
{
    model_rotation = float(int(model_rotation + 5.0f * direction) % 360);
}
void timer_callback(int val)
{
    glutPostRedisplay();
    glutTimerFunc(16, timer_callback, val);
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
    delete materialShader;
    return 0;
}