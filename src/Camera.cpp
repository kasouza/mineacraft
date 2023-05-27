#include "Camera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>

namespace mine {

Camera::Camera(float speed, float sensibility)
    : speed{speed}, sensibility{sensibility} {}

void Camera::move(Camera::Direction direction, float dt) {
    glm::vec3 right{glm::cross(this->front, this->up)};

    float velocity = this->speed * dt;

    switch (direction) {
    case Camera::Direction::UP:
        this->eye += this->up * velocity;
        break;

    case Camera::Direction::DOWN:
        this->eye -= this->up * velocity;
        break;

    case Camera::Direction::RIGHT:
        this->eye += right * velocity;
        break;

    case Camera::Direction::LEFT:
        this->eye -= right * velocity;
        break;

    case Camera::Direction::FORWARD:
        this->eye += this->front * velocity;
        break;

    case Camera::Direction::BACKWARD:
        this->eye -= this->front * velocity;
        break;
    }
}
void Camera::handleMouseMovement(glm::vec2 cursorPos, float dt) {
    static float previousX{cursorPos.x};
    static float previousY{cursorPos.y};

    float deltaX{cursorPos.x - previousX};
    float deltaY{cursorPos.y - previousY};

    glm::vec3 right{glm::cross(this->front, this->up)};
    float velocity { this->sensibility * dt };

    if (deltaX != 0) {
        this->front = glm::rotate(this->front, -(deltaX * velocity), this->up);
    }

    if (deltaY != 0) {
        this->front = glm::rotate(this->front, -(deltaY * velocity), right);
    }

    previousX = cursorPos.x;
    previousY = cursorPos.y;
}

void Camera::setPosition(glm::vec3 position) { this->eye = position; }

glm::mat4 Camera::calculateLookAtMatrix() {
    glm::vec3 center{this->eye + this->front};
    return glm::lookAt(this->eye, center, this->up);
}

} // namespace mine
