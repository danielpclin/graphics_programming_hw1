#include "Common.h"

class Shader {
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            // if geometry shader path is present, also load a geometry shader
            if (geometryPath != nullptr) {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        } catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment, geometry;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // if geometry shader is given, compile geometry shader
        if (geometryPath != nullptr) {
            const char * gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, nullptr);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }
        // shader materialShader
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (geometryPath != nullptr)
            glAttachShader(ID, geometry);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our materialShader now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr)
            glDeleteShader(geometry);

    }
    // activate the materialShader
    // ------------------------------------------------------------------------
    void use() const {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    static void checkCompileErrors(GLuint shader, const std::string& type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
class Camera {
public:
    enum Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    // camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // euler Angles
    float yaw = -90.0f;
    float pitch = 0.0f;
    // camera options
    float movementSpeed = 4.0f;
    float mouseSensitivity = 0.03f;
    float zoom = 45.0f;

    // constructor with vectors
    Camera (glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f):
            position(position), worldUp(up), yaw(yaw), pitch(pitch), front(glm::vec3(0.0f, 0.0f, -1.0f)) {
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera (float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch):
            position(glm::vec3(posX, posY, posZ)), worldUp(glm::vec3(upX, upY, upZ)), yaw(yaw), pitch(pitch),
            front(glm::vec3(0.0f, 0.0f, -1.0f)) {
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    // processes input received from any keyboard-like input system.
    // Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboard(Movement direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
        if (direction == UP)
            position += up * velocity;
        if (direction == DOWN)
            position -= up * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float x_offset, float y_offset, GLboolean constrainPitch = true) {
        yaw   += x_offset * mouseSensitivity;
        pitch += y_offset * mouseSensitivity;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        // update front, right and up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float y_offset) {
        zoom -= static_cast<float>(y_offset);
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() {
        // calculate the new front vector
        glm::vec3 _front;
        _front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        _front.y = sin(glm::radians(pitch));
        _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(_front);

        // also re-calculate the right and up vector

        // normalize the vectors, because their length gets closer to 0 the more you look up or down
        // which results in slower movement.
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};
class Texture {
private:
    struct TextureData {
        TextureData(): width(0), height(0), data(nullptr) {}

        int width;
        int height;
        unsigned char* data;
    };

public:
    GLuint texture = 0;

    // load a png image and return a TextureData structure with raw data
    // not limited to png format. works with any image format that is RGBA-32bit
    static TextureData loadImg(const std::string& imgFilePath) {
        TextureData textureData;
        int components;

        // load the textureData with stb image, force RGBA (4 components required)
        stbi_uc *data = stbi_load(imgFilePath.c_str(), &textureData.width, &textureData.height, &components, 4);

        // is the image successfully loaded?
        if (data != nullptr) {
            // copy the raw data
            size_t dataSize = textureData.width * textureData.height * 4 * sizeof(unsigned char);
            textureData.data = new unsigned char[dataSize];
            memcpy(textureData.data, data, dataSize);

            // mirror the image vertically to comply with OpenGL convention
            for (size_t i = 0; i < textureData.width; ++i) {
                for (size_t j = 0; j < textureData.height / 2; ++j) {
                    for (size_t k = 0; k < 4; ++k) {
                        size_t coord1 = (j * textureData.width + i) * 4 + k;
                        size_t coord2 = ((textureData.height - j - 1) * textureData.width + i) * 4 + k;
                        std::swap(textureData.data[coord1], textureData.data[coord2]);
                    }
                }
            }

            // release the loaded image
            stbi_image_free(data);
        }

        return textureData;
    }

    explicit Texture(const std::string& filename) {
        TextureData textureData = loadImg(filename);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
//        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        std::cout << "Loaded texture \"" << filename << "\"" << std::endl;

        delete textureData.data;
    }
    ~Texture() {
        glDeleteTextures(1, &texture);
    }

    void bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    static void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};
class Model {
public:
    GLuint vao = 0; // vertex array object
    GLuint vbo = 0; // vertex buffer object
    int vertexCount = 0;

    // Load .obj model
    explicit Model(const std::string& filename) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

        if (!warn.empty())
            std::cout << warn << std::endl;
        if (!err.empty())
            std::cout << err << std::endl;
        if (!ret)
            exit(1);


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
                vertexCount += fv;
            }
        }

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

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

        std::cout << "Loaded model \"" << filename << "\", " << vertexCount << " vertices" << std::endl;
    }

    void bind() const {
        glBindVertexArray(vao);
    }
};
class Scene {
public:
    enum Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
    };
    struct KeyFrame {
        glm::vec3 translation;
        glm::quat rotation;
        glm::vec3 scale;
    };
    struct SceneNode {
        SceneNode(Model *model, Shader *shader, Texture *texture, glm::vec3 color,
                  glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, std::vector<KeyFrame> keyFrames = {}):
                model(model), shader(shader), texture(texture), translation(translation), rotation(rotation), scale(scale),
                keyFrames(std::move(keyFrames)), color(color)  {}
        SceneNode(Model *model, Shader *shader, Texture *texture, glm::vec3 color, int parent,
                  glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale, std::vector<KeyFrame> keyFrames = {}):
                model(model), shader(shader), texture(texture), translation(translation), rotation(rotation), scale(scale),
                parent(parent), keyFrames(std::move(keyFrames)), color(color) {}
        Model* model;
        Shader* shader;
        Texture* texture;
        glm::vec3 color;
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::vec3 animationTranslation{};
        glm::quat animationRotation{glm::vec3(0.0)};
        glm::vec3 animationScale{1.0};
        glm::mat4 matrix{1.0}; // translation & rotation
//        glm::mat4 animationMatrix{1.0}; // translation & rotation
        std::vector<KeyFrame> keyFrames;
        int parent = -1;
    };

private:
    const int32_t KEYFRAME_TIME = 500;
    int lastTime = 0;
    glm::vec3 translation;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 matrix;
    std::vector<SceneNode> nodes = {};

public:
    Scene(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale):
            translation(translation), rotation(rotation), scale(scale), matrix(calculateSceneMatrix()) {}
    Scene(): translation(0.0f), rotation(0.0f), scale(1.0f), matrix(calculateSceneMatrix()) {}
    void addNodes(const std::vector<SceneNode>& _nodes) { // add nodes to scene
        nodes.insert(nodes.end(), _nodes.begin(), _nodes.end());
        updateMatrices();
    }
    SceneNode getNode(int position) { // get node from scene
        return nodes[position];
    }
    void updateNode(int position, SceneNode node) { // update node
        nodes[position] = std::move(node);
        updateMatrices();
    }
    void draw() { // render the scene
        for (auto& node: nodes) {
            glm::mat4 model_matrix = glm::mat4(matrix);
            model_matrix = model_matrix * node.matrix;
            model_matrix = glm::scale(model_matrix, node.scale);
            model_matrix = glm::scale(model_matrix, node.animationScale);
            node.shader->use();
            node.shader->setMat4("model", model_matrix);
            if(node.texture){
                node.texture->bind(0);
            }else{
                node.shader->setVec3("material.ambient", node.color);
                node.shader->setVec3("material.diffuse", node.color);
            }
            node.model->bind();
            glDrawArrays(GL_TRIANGLES, 0, node.model->vertexCount);
        }
    }
    void updateSceneVectors(glm::vec3 _translation, glm::vec3 _rotation, glm::vec3 _scale) {
        translation = _translation;
        rotation = _rotation;
        scale = _scale;
        updateMatrices();
    }
    void updateSceneTranslation(glm::vec3 _translation) {
        translation = _translation;
        updateMatrices();
    }
    void updateSceneRotation(glm::vec3 _rotation) {
        rotation = _rotation;
        updateMatrices();
    }
    void updateSceneScale(glm::vec3 _scale) {
        scale = _scale;
        updateMatrices();
    }
    void animate(int deltaTime) {
        int keyFrameId = (lastTime + deltaTime) / KEYFRAME_TIME;
        float timeAfterKeyFrame = static_cast<float>(((lastTime + deltaTime) % KEYFRAME_TIME)) / KEYFRAME_TIME;
        for (auto &node: nodes){
            if (!node.keyFrames.empty()){
                int nodeKeyFrameId = keyFrameId % static_cast<int>(node.keyFrames.size());
                if (nodeKeyFrameId == 0){
                    node.animationTranslation = glm::mix(glm::vec3(0.0),
                                                         node.keyFrames[nodeKeyFrameId].translation,
                                                         timeAfterKeyFrame);
                    node.animationRotation = glm::mix(glm::quat(glm::radians(glm::vec3(0.0))),
                                                      node.keyFrames[nodeKeyFrameId].rotation,
                                                      timeAfterKeyFrame);
                    node.animationScale = glm::mix(glm::vec3(1.0),
                                                   node.keyFrames[nodeKeyFrameId].scale,
                                                   timeAfterKeyFrame);
                } else {
                    node.animationTranslation = glm::mix(node.keyFrames[nodeKeyFrameId - 1].translation,
                                                         node.keyFrames[nodeKeyFrameId].translation,
                                                         timeAfterKeyFrame);
                    node.animationRotation = glm::mix(node.keyFrames[nodeKeyFrameId - 1].rotation,
                                                      node.keyFrames[nodeKeyFrameId].rotation,
                                                      timeAfterKeyFrame);
                    node.animationScale = glm::mix(node.keyFrames[nodeKeyFrameId - 1].scale,
                                                   node.keyFrames[nodeKeyFrameId].scale,
                                                   timeAfterKeyFrame);
                }
            }
        }
        updateMatrices();
        lastTime += deltaTime;
    }
    void processKeyboard(Movement direction, float deltaTime) {
        float velocity = 4.0f * deltaTime;
        if (direction == FORWARD)
            translation += glm::vec3(0.0, 0.0, 1.0) * velocity;
        if (direction == BACKWARD)
            translation += glm::vec3(0.0, 0.0, -1.0) * velocity;
        if (direction == LEFT)
            translation += glm::vec3(-1.0, 0.0, 0.0) * velocity;
        if (direction == RIGHT)
            translation += glm::vec3(1.0, 0.0, 0.0) * velocity;
        matrix = calculateSceneMatrix();
    }
private:
    glm::mat4 calculateSceneMatrix() {
        glm::mat4 scene_model_matrix(1.0f);
        scene_model_matrix = glm::translate(scene_model_matrix, translation);
        scene_model_matrix = scene_model_matrix * glm::mat4(glm::quat(glm::radians(rotation)));
        scene_model_matrix = glm::scale(scene_model_matrix, scale);
        return scene_model_matrix;
    }
    void updateMatrices() { // calculate all mvp matrices
        matrix = calculateSceneMatrix();
        for (auto& node: nodes) {
            glm::mat4 model_matrix;
            if (node.parent == -1){
                model_matrix = glm::mat4(1.0);
            } else {
                model_matrix = glm::mat4(nodes[node.parent].matrix);
            }
            model_matrix = glm::translate(model_matrix, node.translation);
            model_matrix = glm::translate(model_matrix, node.animationTranslation);
            model_matrix = model_matrix * glm::mat4(glm::quat(glm::radians(node.rotation)));
            model_matrix = model_matrix * glm::mat4(node.animationRotation);
            node.matrix = model_matrix;
        }
    }
};

Shader *materialShader;
Shader *textureShader;
Model* capsule;
Model* cube;
Model* cylinder;
Model* plane;
Model* sphere;
Texture* texture;
Scene *scene;
Camera *camera;
glm::mat4 projection_matrix(1.0f);
float model_rotation = 0.0f;

const uint32_t SCREEN_WIDTH = 1000;
const uint32_t SCREEN_HEIGHT = 800;
float mouse_last_x = 0;
float mouse_last_y = 0;
bool firstMouse = true;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//imgui state
bool run_animation = true;
bool capture_mouse = false;


static void GLAPIENTRY debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                            const GLchar* message, const void* userParam ) {
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        return;
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             type, severity, message);
}
void init() {
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    camera = new Camera(glm::vec3(-3.0, 1.0, 5.0), glm::vec3(0.0, 1.0, 0.0), -60.0, -7.0);

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
    texture = new Texture("texture/block.png");

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
    texture->bind(0);

    // setup scene
    scene = new Scene(glm::vec3(0.0, 0.0, 0.0),
                      glm::vec3(0.0, 0.0, 0.0),
                      glm::vec3(1.0));
    scene->addNodes({
        Scene::SceneNode(cube, textureShader, texture, glm::vec3(1.0), -1, // body id 0
                         glm::vec3(0.0, 0.0, 0.0),
                         glm::vec3(0.0, 0.0, 0.0),
                         glm::vec3(0.8, 1.0, 0.4), {
                                 {glm::vec3(0.0, -0.5, 0.0),
                                  glm::quat(glm::radians(glm::vec3(0.0, 0.0, 0.0))),
                                  glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(1.5, 2.0, 0.0),
                                  glm::quat(glm::radians(glm::vec3(0.0, 179.9, 0.0))),
                                  glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(3.0, 0.0, 0.0),
                                  glm::quat(glm::radians(glm::vec3(0.0, 359.8, 0.0))),
                                  glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(3.0, -0.5, 0.0),
                                  glm::quat(glm::radians(glm::vec3(0.0, 359.8, 0.0))),
                                  glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(1.5, 3.0, 0.0),
                                  glm::quat(glm::radians(glm::vec3(0.0, 179.9, 0.0))),
                                  glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                  glm::quat(glm::radians(glm::vec3(0.0, 0.0, 0.0))),
                                  glm::vec3(1.0, 1.0, 1.0)},
        }),
        Scene::SceneNode(sphere, materialShader, nullptr, glm::vec3(0.99, 0.5, 0.44), 0, // left hand top joint id 1
                         glm::vec3(0.46, 0.4, 0.0),
                         glm::vec3(0.0, 0.0, 6.0),
                         glm::vec3(0.25, 0.25, 0.25), {
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(40.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(-180.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(0.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                         }),
        Scene::SceneNode(cube, materialShader, nullptr, glm::vec3(0.49, 0.69, 0.84), 1, // left hand top id 2
                         glm::vec3(0.03, -0.2, 0.0),
                         glm::vec3(0.0, 0.0, 0.0),
                         glm::vec3(0.13, 0.5, 0.13), {}),
        Scene::SceneNode(sphere, materialShader, nullptr, glm::vec3(0.7, 0.88, 0.38), 2, // left hand bottom joint id 3
                         glm::vec3(0.0, -0.22, 0.0),
                         glm::vec3(-30.0, 0.0, -3.0),
                         glm::vec3(0.22, 0.22, 0.22), {
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(-80.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(-40.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(0.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                         }),
        Scene::SceneNode(cube, materialShader, nullptr, glm::vec3(0.74, 0.49, 0.75), 3, // left hand bottom id 4
                         glm::vec3(0.0, -0.2, 0.0),
                         glm::vec3(0.0, 0.0, 0.0),
                         glm::vec3(0.13, 0.5, 0.13), {}),
        Scene::SceneNode(sphere, materialShader, nullptr, glm::vec3(1.0, 0.71, 0.35), 0, // right hand top joint id 5
                         glm::vec3(-0.46, 0.4, 0.0),
                         glm::vec3(0.0, 0.0, -6.0),
                         glm::vec3(0.25, 0.25, 0.25), {
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(40.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(-180.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(0.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                         }),
        Scene::SceneNode(cube, materialShader, nullptr, glm::vec3(1.0, 0.93, 0.4), 5, // right hand top id 6
                         glm::vec3(-0.03, -0.2, 0.0),
                         glm::vec3(0.0, 0.0, 0.0),
                         glm::vec3(0.13, 0.5, 0.13), {}),
        Scene::SceneNode(sphere, materialShader, nullptr, glm::vec3(0.75, 0.73, 0.86), 6, // right hand bottom joint id 7
                         glm::vec3(0.0, -0.22, 0.0),
                         glm::vec3(-30.0, 0.0, 3.0),
                         glm::vec3(0.22, 0.22, 0.22), {
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(-80.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(-40.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(0.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                         }),
        Scene::SceneNode(cube, materialShader, nullptr, glm::vec3(0.99, 0.8, 0.9), 7, // right hand bottom id 8
                         glm::vec3(0.0, -0.2, 0.0),
                         glm::vec3(0.0, 0.0, 0.0),
                         glm::vec3(0.13, 0.5, 0.13), {}),
        Scene::SceneNode(sphere, materialShader, nullptr, glm::vec3(0.7, 0.83, 1.0), 0, // head id 9
                         glm::vec3(0.0, 0.7, 0.0),
                         glm::vec3(0.0, 0.0, 0.0),
                         glm::vec3(0.5, 0.5, 0.5), {}),
        Scene::SceneNode(sphere, materialShader, nullptr, glm::vec3(0.2), 9, // left eye id 10
                         glm::vec3(0.1, 0.08, 0.21),
                         glm::vec3(0.0, 0.0, 0.0),
                         glm::vec3(0.05, 0.07, 0.05), {}),
        Scene::SceneNode(sphere, materialShader, nullptr, glm::vec3(0.2), 9, // right eye id 11
                         glm::vec3(-0.1, 0.08, 0.21),
                         glm::vec3(0.0, 0.0, 0.0),
                         glm::vec3(0.05, 0.07, 0.05), {}),
        Scene::SceneNode(sphere, materialShader, nullptr, glm::vec3(0.92, 0.33, 0.27), 0, // left leg top joint id 12
                         glm::vec3(0.2, -0.5, 0.0),
                         glm::vec3(-0.0, 0.0, 0.0),
                         glm::vec3(0.2, 0.2, 0.2), {
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(-80.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(0.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(0.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                         }),
        Scene::SceneNode(cube, materialShader, nullptr, glm::vec3(0.92, 0.33, 0.27), 12, // left leg top id 13
                         glm::vec3(0.0, -0.27, 0.0),
                         glm::vec3(0.0, 0.0, 0.0),
                         glm::vec3(0.13, 0.5, 0.13), {}),
        Scene::SceneNode(sphere, materialShader, nullptr, glm::vec3(0.94, 0.61, 0.13), 13, // left leg bottom joint id 14
                         glm::vec3(0.0, -0.28, 0.0),
                         glm::vec3(-0.0, 0.0, 0.0),
                         glm::vec3(0.2, 0.2, 0.2), {
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(140.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(0.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(0.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                         }),
        Scene::SceneNode(cube, materialShader, nullptr, glm::vec3(0.93, 0.75, 0.2), 14, // left leg bottom id 15
                         glm::vec3(0.0, -0.22, 0.0),
                         glm::vec3(0.0, 0.0, 0.0),
                         glm::vec3(0.13, 0.4, 0.13), {}),
        Scene::SceneNode(sphere, materialShader, nullptr, glm::vec3(0.93, 0.88, 0.36), 0, // right leg top joint id 16
                         glm::vec3(-0.2, -0.5, 0.0),
                         glm::vec3(-0.0, 0.0, 0.0),
                         glm::vec3(0.2, 0.2, 0.2), {
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(-80.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(0.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(0.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                         }),
        Scene::SceneNode(cube, materialShader, nullptr, glm::vec3(0.74, 0.81, 0.2), 16, // right leg top id 17
                         glm::vec3(0.0, -0.27, 0.0),
                         glm::vec3(0.0, 0.0, 0.0),
                         glm::vec3(0.13, 0.5, 0.13), {}),
        Scene::SceneNode(sphere, materialShader, nullptr, glm::vec3(0.53, 0.74, 0.27), 17, // right leg bottom joint id 18
                         glm::vec3(0.0, -0.28, 0.0),
                         glm::vec3(-0.0, 0.0, 0.0),
                         glm::vec3(0.2, 0.2, 0.2), {
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(140.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(0.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                                 {glm::vec3(0.0, 0.0, 0.0),
                                         glm::quat(glm::radians(glm::vec3(0.0, 0.0, 0.0))),
                                         glm::vec3(1.0, 1.0, 1.0)},
                         }),
        Scene::SceneNode(cube, materialShader, nullptr, glm::vec3(0.15, 0.68, 0.94), 18, // right leg bottom id 19
                         glm::vec3(0.0, -0.22, 0.0),
                         glm::vec3(0.0, 0.0, 0.0),
                         glm::vec3(0.13, 0.4, 0.13), {}),
    });
}
void draw() {
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

    if (run_animation)
        scene->animate(static_cast<int>(deltaTime * 1000));
    scene->draw();

}
void toggle_mouse(GLFWwindow* window) {
    if (!capture_mouse){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        firstMouse = true;
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    capture_mouse = !capture_mouse;
}
// whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset) {
    model_rotation = static_cast<float>(int(model_rotation + 2.0f * y_offset + 360) % 360);
    scene->updateSceneRotation(glm::vec3(0.0f, model_rotation, 0.0f));
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    projection_matrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    // Re-render the scene because the current frame was drawn for the old resolution
    draw();
    glfwSwapBuffers(window);
}
static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}
void mouse_callback(GLFWwindow* window, double x_pos_in, double y_pos_in) {
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
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (action == GLFW_PRESS && key == GLFW_KEY_E){
        if (capture_mouse)
            toggle_mouse(window);
    }
}
void mouse_button_callback(GLFWwindow* window) {
    toggle_mouse(window);
}
void process_input(GLFWwindow* window) {
    // wasd / ctrl/ space to move camera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(Camera::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(Camera::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(Camera::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(Camera::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera->processKeyboard(Camera::DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->processKeyboard(Camera::UP, deltaTime);
    // ijkl to move model
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        scene->processKeyboard(Scene::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        scene->processKeyboard(Scene::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        scene->processKeyboard(Scene::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        scene->processKeyboard(Scene::RIGHT, deltaTime);
}
void prepare_imgui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    {
        // right click context menu
        if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && ImGui::IsMouseClicked(1))
            ImGui::OpenPopup("contextmenu");

        if (ImGui::BeginPopup("contextmenu")) {
            if (run_animation){
                if (ImGui::Button("Stop animation"))
                    run_animation = !run_animation;
            }else{
                if (ImGui::Button("Run animation"))
                    run_animation = !run_animation;
            }

            ImGui::EndPopup();
        }
    }

    {
        ImGui::Begin("Info");

        if (ImGui::SliderFloat("model rotation", &model_rotation, 0.0f, 360.0f))
            scene->updateSceneRotation(glm::vec3(0.0f, model_rotation, 0.0f));

        ImGui::Text("Application %.1f FPS", io.Framerate);
        ImGui::Text("Left click to mount/unmount camera");
        ImGui::Text("E to unmount camera");
        ImGui::Text("WASD, ctrl, space to move camera");
        ImGui::Text("IJKL to move scene(model)");
        ImGui::Text("scrollwheel to rotate scene(model)");
        ImGui::Text("Right click to open context menu");
        if (run_animation){
            if (ImGui::Button("Stop animation"))
                run_animation = !run_animation;
        }else{
            if (ImGui::Button("Run animation"))
                run_animation = !run_animation;
        }

        ImGui::End();
    }

}
int main(int argc, char *argv[]) {
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graphics programming assignment 1", nullptr, nullptr);
    if (!window) {
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
    if (GLEW_OK != err) {
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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // setup viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    projection_matrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    init();

    while (!glfwWindowShouldClose(window)) {
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