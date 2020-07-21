#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARDS,
    LEFT,
    RIGHT
};

// https://learnopengl.com/Getting-started/Camera
class Camera {
    public:
        Camera();
        Camera(glm::vec3 Position);

        float MovementSpeed;
        float MouseSensitivity;

        glm::mat4 GetViewMatrix();

        void Move(CameraMovement Direction, float DeltaTime);
        void MouseMovement(float xOffset, float yOffset);

    private:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        float Yaw;
        float Pitch;

        void UpdateCameraVectors();
};

#endif