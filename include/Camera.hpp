#ifndef KASOUZA_MINECRAFT_INCLUDE_CAMERA_HPP
#define KASOUZA_MINECRAFT_INCLUDE_CAMERA_HPP

#include <glm/ext.hpp>
#include <glm/vec3.hpp>

namespace mine {

class Camera {
  public:
    enum class Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN,
    };

    Camera(float speed = 1.0f, float sensibility = 1.0f);

    void move(Camera::Direction direction, float dt = 1.0f);
    void handleMouseMovement(glm::vec2 cursorPos, float dt = 1.0f);
    void setPosition(glm::vec3 position);
    glm::mat4 calculateLookAtMatrix();

  private:
    glm::vec3 eye{};
    glm::vec3 front{0.0, 0.0, 1.0};
    glm::vec3 up{0.0f, 1.0f, 0.0f};

    float speed{};
    float sensibility{};
};

} // namespace mine

#endif
