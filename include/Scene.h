#ifndef SCENE_H
#define SCENE_H
class Scene
{
public:
    struct SceneNode
    {
        SceneNode(Model *model, Shader *shader, Texture *texture, glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale):
                model(model), shader(shader), texture(texture), translation(translation), rotation(rotation), scale(scale), matrix(1.0f) {}
        Model* model;
        Shader* shader;
        Texture* texture;
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 matrix;
    };

private:
    glm::vec3 translation;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 matrix;
    std::vector<SceneNode> nodes = {};

public:

    Scene(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale):
        translation(translation), rotation(rotation), scale(scale), matrix(calculateMatrix()) {}
    Scene(): translation(0.0f), rotation(0.0f), scale(1.0f), matrix(calculateMatrix()) {}

    void addNode(SceneNode node)
    {
        nodes.push_back(node);
    }

    void updateMatrices()
    {
        matrix = calculateMatrix();
        for (auto& node: nodes) {
            glm::mat4 model_matrix(matrix);
            model_matrix = glm::translate(model_matrix, node.translation);
            model_matrix = model_matrix * glm::mat4(glm::quat(glm::radians(node.rotation)));
            model_matrix = glm::scale(model_matrix, node.scale);
            node.matrix = model_matrix;
        }
    }

    void draw()
    {
        for (auto& node: nodes) {
            node.shader->use();
            node.shader->setMat4("model", node.matrix);
            node.model->bind();
            glDrawArrays(GL_TRIANGLES, 0, node.model->vertexCount);
        }
    }

    void updateTranslation(glm::vec3 _translation)
    {
        translation = _translation;
        updateMatrices();
    }
    void updateRotation(glm::vec3 _rotation)
    {
        rotation = _rotation;
        updateMatrices();
    }
    void updateScale(glm::vec3 _scale)
    {
        scale = _scale;
        updateMatrices();
    }
    glm::mat4 calculateMatrix()
    {
        glm::mat4 scene_model_matrix(1.0f);
        scene_model_matrix = glm::translate(scene_model_matrix, translation);
        scene_model_matrix = scene_model_matrix * glm::mat4(glm::quat(glm::radians(rotation)));
        scene_model_matrix = glm::scale(scene_model_matrix, scale);
        return scene_model_matrix;
    }


};

#endif //SCENE_H
