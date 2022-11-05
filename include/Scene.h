#ifndef SCENE_H
#define SCENE_H
class Scene {
public:
    struct KeyFrame {
        glm::vec3 translation;
        glm::quat rotation;
        glm::vec3 scale;
    };
    struct SceneNode {
        SceneNode(Model *model, Shader *shader, Texture *texture,
                  glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale):
            model(model), shader(shader), texture(texture), translation(translation), rotation(rotation), scale(scale),
            matrix(1.0f) {}
        SceneNode(Model *model, Shader *shader, Texture *texture, int parent,
                  glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale):
            model(model), shader(shader), texture(texture), translation(translation), rotation(rotation), scale(scale),
            matrix(1.0f), parent(parent) {}
        Model* model;
        Shader* shader;
        Texture* texture;
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;
        glm::mat4 matrix;
        glm::vec3 animationTranslation{};
        glm::quat animationRotation{glm::vec3(0.0)};
        glm::vec3 animationScale{1.0};
        std::vector<KeyFrame> keyFrames{
            {glm::vec3(0.5),glm::quat(glm::vec3(80.0)),glm::vec3(1.0)},
            {glm::vec3(0.8),glm::quat(glm::vec3(160.0)),glm::vec3(1.0)},
            {glm::vec3(0.0),glm::quat(glm::vec3(0.0)),glm::vec3(1.0)},
        };
        int parent = -1;
    };

private:
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
            glm::mat4 model_matrix(node.matrix);
            model_matrix = glm::translate(model_matrix, node.animationTranslation);
            model_matrix = model_matrix * glm::mat4(node.animationRotation);
            model_matrix = glm::scale(model_matrix, node.scale);
            model_matrix = glm::scale(model_matrix, node.animationScale);
            node.shader->use();
            node.shader->setMat4("model", model_matrix);
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
        int keyFrameId = (lastTime + deltaTime) / 1000;
        float timeAfterKeyFrame = static_cast<float>(((lastTime + deltaTime) % 1000))/1000;
        for (auto &node: nodes){
            if (!node.keyFrames.empty()){
                int nodeKeyFrameId = keyFrameId % static_cast<int>(node.keyFrames.size());
                if (nodeKeyFrameId == 0){
                    node.animationTranslation = glm::mix(glm::vec3(0.0), node.keyFrames[nodeKeyFrameId].translation, timeAfterKeyFrame);
                    node.animationRotation = glm::mix(glm::quat(glm::vec3(0.0)), node.keyFrames[nodeKeyFrameId].rotation, timeAfterKeyFrame);
                    node.animationScale = glm::mix(glm::vec3(1.0), node.keyFrames[nodeKeyFrameId].scale, timeAfterKeyFrame);
                } else {
                    node.animationTranslation = glm::mix(node.keyFrames[nodeKeyFrameId - 1].translation, node.keyFrames[nodeKeyFrameId].translation, timeAfterKeyFrame);
                    node.animationRotation = glm::mix(node.keyFrames[nodeKeyFrameId - 1].rotation, node.keyFrames[nodeKeyFrameId].rotation, timeAfterKeyFrame);
                    node.animationScale = glm::mix(node.keyFrames[nodeKeyFrameId - 1].scale, node.keyFrames[nodeKeyFrameId].scale, timeAfterKeyFrame);
                }
            }
        }
        lastTime += deltaTime;
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
                model_matrix = glm::mat4(matrix);
            } else {
                model_matrix = glm::mat4(nodes[node.parent].matrix);
            }
            model_matrix = glm::translate(model_matrix, node.translation);
            model_matrix = model_matrix * glm::mat4(glm::quat(glm::radians(node.rotation)));
            node.matrix = model_matrix;
        }
    }
};

#endif //SCENE_H
