#ifndef CAMERA_H
#define CAMERA_H

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  4.0f;
const float SENSITIVITY =  0.03f;
const float ZOOM        =  45.0f;

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
    float yaw;
    float pitch;
    // camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    // constructor with vectors
    Camera (glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH):
            position(position), worldUp(up), yaw(yaw), pitch(pitch), front(glm::vec3(0.0f, 0.0f, -1.0f)),
            movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera (float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch):
            position(glm::vec3(posX, posY, posZ)), worldUp(glm::vec3(upX, upY, upZ)), yaw(yaw), pitch(pitch),
            front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
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
#endif //CAMERA_H
