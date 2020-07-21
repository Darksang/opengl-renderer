#include "camera.h"

Camera::Camera() {
    Position = glm::vec3(0.0f, 0.0f, 0.0f);
    Front = glm::vec3(0.0f, 0.0f, -1.0f);
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    Yaw = -90.0f;
    Pitch = 0.0f;

    UpdateCameraVectors();

    MovementSpeed = 2.5f;
    MouseSensitivity = 0.1f;
}

Camera::Camera(glm::vec3 Position) {
    this->Position = Position;
    Front = glm::vec3(0.0f, 0.0f, -1.0f);
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    Yaw = -90.0f;
    Pitch = 0.0f;

    UpdateCameraVectors();

    MovementSpeed = 2.5f;
    MouseSensitivity = 0.1f;
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::Move(CameraMovement Direction, float DeltaTime) {
    float Velocity = MovementSpeed * DeltaTime;

    if (Direction == FORWARD)
        Position += Front * Velocity;
        
    if (Direction == BACKWARDS)
        Position -= Front * Velocity;

    if (Direction == LEFT)
        Position -= Right * Velocity;

    if (Direction == RIGHT)
        Position += Right * Velocity;
}

void Camera::MouseMovement(float xOffset, float yOffset) {
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
    glm::vec3 NewFront;

    NewFront.x = glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
    NewFront.y = glm::sin(glm::radians(Pitch));
    NewFront.z = glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));

    Front = glm::normalize(NewFront);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}