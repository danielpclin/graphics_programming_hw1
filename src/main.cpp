#include "Common.h"

class Model
{
public:
    GLuint vao;			// vertex array object
    GLuint vbo;			// vertex buffer object

    int materialId;
    int vertexCount;
    GLuint texture;
};

Model model;
Program *program;
glm::mat4 projection_matrix(1.0f);
float model_rotation = 0.0f;

// Load .obj model
void load_model(const std::string& filename, const std::string& texture_filename)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());
    if (!warn.empty()) {
        std::cout << warn << std::endl;
    }
    if (!err.empty()) {
        std::cout << err << std::endl;
    }
    if (!ret) {
        exit(1);
    }

    std::vector<float> vertices, normals, tex_coords;  // if OBJ preserves vertex order, you can use element array buffer for memory efficiency
    for (auto & shape : shapes) {  // for 'ladybug.obj', there is only one object
        int index_offset = 0;
        for (int f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
            int fv = shape.mesh.num_face_vertices[f];
            for (int v = 0; v < fv; ++v) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
                normals.push_back(attrib.normals[3 * idx.normal_index + 0]);
                normals.push_back(attrib.normals[3 * idx.normal_index + 1]);
                normals.push_back(attrib.normals[3 * idx.normal_index + 2]);
                tex_coords.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                tex_coords.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
            }
            index_offset += fv;
            model.vertexCount += fv;
        }
    }

    glGenVertexArrays(1, &model.vao);
    glBindVertexArray(model.vao);

    glGenBuffers(1, &model.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model.vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) + normals.size() * sizeof(float) + tex_coords.size() * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), normals.size() * sizeof(float), normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) + normals.size() * sizeof(float), tex_coords.size() * sizeof(float), tex_coords.data());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(vertices.size() * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(vertices.size() * sizeof(float) + normals.size() * sizeof(float)));
    glEnableVertexAttribArray(2);


    shapes.clear();
    shapes.shrink_to_fit();
    materials.clear();
    materials.shrink_to_fit();
    vertices.clear();
    vertices.shrink_to_fit();
    normals.clear();
    normals.shrink_to_fit();
    tex_coords.clear();
    tex_coords.shrink_to_fit();

    std::cout << "Load " << model.vertexCount << " vertices" << std::endl;

    if (texture_filename.empty())
        return;

    TextureData textureData = loadImg(texture_filename.c_str());

    glGenTextures(1, &model.texture);
    glBindTexture(GL_TEXTURE_2D, model.texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete textureData.data;
}

void init()
{
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    program = new Program("shader/vertex.glsl", "shader/fragment.glsl");
    load_model("obj/Capsule.obj", "");
    program->use();

    program->setVec3("lightPos", 3.0f, 5.0f, 10.0f);
    program->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    program->setVec3("objectColor", 1.0f, 0.5f, 0.3f);

}

// GLUT callback. Called to draw the scene.
void render_callback()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // projection
    program->setMat4("projection", projection_matrix);

    // view
    glm::mat4 view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
//    view_matrix = glm::rotate(view_matrix, glm::radians(model_rotation), glm::vec3(0, 1, 0));

    program->setMat4("view", view_matrix);

    // model 1
    glm::mat4 model_matrix(1.0f);
    model_matrix = glm::rotate(model_matrix, glm::radians(model_rotation), glm::vec3(0, 1, 0));
    model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, 0.0f));
    model_matrix = glm::rotate(model_matrix, glm::radians(60.0f), glm::vec3(1, 0, 0));
    model_matrix = glm::rotate(model_matrix, glm::radians(0.0f), glm::vec3(0, 1, 0));
    model_matrix = glm::rotate(model_matrix, glm::radians(0.0f), glm::vec3(0, 0, 1));
    model_matrix = glm::scale(model_matrix, glm::vec3(0.5f));

    program->setMat4("model", model_matrix);
    glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);

    glm::mat4 model2_matrix(1.0f);
    model2_matrix = glm::rotate(model2_matrix, glm::radians(model_rotation), glm::vec3(0, 1, 0));
    model2_matrix = glm::translate(model2_matrix, glm::vec3(1.0f, 0.0f, 0.0f));
    model2_matrix = glm::rotate(model2_matrix, glm::radians(0.0f), glm::vec3(1, 0, 0));
    model2_matrix = glm::rotate(model2_matrix, glm::radians(0.0f), glm::vec3(0, 1, 0));
    model2_matrix = glm::rotate(model2_matrix, glm::radians(60.0f), glm::vec3(0, 0, 1));
    model2_matrix = glm::scale(model2_matrix, glm::vec3(0.5f));

    program->setMat4("model", model2_matrix);
    glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);

    glm::mat4 model3_matrix(1.0f);
    model3_matrix = glm::rotate(model3_matrix, glm::radians(model_rotation), glm::vec3(0, 1, 0));
    model3_matrix = glm::translate(model3_matrix, glm::vec3(0.0f, 0.0f, 1.0f));
    model3_matrix = glm::rotate(model3_matrix, glm::radians(0.0f), glm::vec3(1, 0, 0));
    model3_matrix = glm::rotate(model3_matrix, glm::radians(0.0f), glm::vec3(0, 1, 0));
    model3_matrix = glm::rotate(model3_matrix, glm::radians(60.0f), glm::vec3(0, 0, 1));
    model3_matrix = glm::scale(model3_matrix, glm::vec3(0.5f));

    program->setMat4("model", model3_matrix);
    glDrawArrays(GL_TRIANGLES, 0, model.vertexCount);




    glutSwapBuffers();
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
    delete program;
    return 0;
}