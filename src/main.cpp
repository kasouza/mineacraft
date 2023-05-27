#include "Camera.hpp"
#include "Program.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "opengl/GLBuffer.hpp"
#include "opengl/ShaderProgram.hpp"
#include "opengl/VertexArray.hpp"
#include "opengl/Window.hpp"
#include "opengl/gl_includes.hpp"

#include <atomic>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <mutex>
#include <ostream>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <vector>

inline void clearScreen() {
    glClearColor(0.2f, 0.3f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

float x_pos = 0.0f;
float look_at_x = 0.0f;

void events(mine::Program &program, mine::Camera &camera) {
    static std::map<int, mine::Camera::Direction> keyToDirection{
        {GLFW_KEY_W, mine::Camera::Direction::FORWARD},
        {GLFW_KEY_S, mine::Camera::Direction::BACKWARD},

        {GLFW_KEY_A, mine::Camera::Direction::LEFT},
        {GLFW_KEY_D, mine::Camera::Direction::RIGHT},

        {GLFW_KEY_Z, mine::Camera::Direction::UP},
        {GLFW_KEY_X, mine::Camera::Direction::DOWN},
    };

    mine::opengl::Window &window = program.getWindow();
    window.pollEvents();

    if (window.isKeyPressed(GLFW_KEY_ESCAPE) || window.shouldClose()) {
        program.stop();
    }

    for (auto [key, direction] : keyToDirection) {
        if (window.isKeyPressed(key)) {
            camera.move(direction);
        }
    }

    camera.handleMouseMovement(window.getCursorPos());
}

void render(mine::Program &program, mine::opengl::VertexArray &vao,
            mine::opengl::ShaderProgram &shaderProgram, mine::Camera &camera) {
    clearScreen();

    shaderProgram.use();

    vao.bind();

    const mine::opengl::Window &window{program.getWindow()};

    float width{static_cast<float>(window.getWidth())};
    float height{static_cast<float>(window.getHeight())};

    float near{0.1};
    float far{100.0};

    glm::mat4 projection{
        glm::perspective(glm::radians(45.0f), width / height, near, far)};

    glm::mat4 view{camera.calculateLookAtMatrix()};
    glm::mat4 model{glm::mat4(1.0f)};

    glm::mat4 mvp{projection * view * model};

    shaderProgram.uniform<3>("offset", glm::vec3{-0.0f, 0.5f, 0.2f});
    shaderProgram.uniform<4>("color", glm::vec4{1.0f, 0.5f, 0.2f, 1.0f});
    shaderProgram.uniform<4>("mvp", mvp);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    shaderProgram.unuse();

    vao.unbind();

    program.getWindow().swapBuffers();
}

void init() {
    atexit([]() { glfwTerminate(); });
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(1);
    }
}

int main() {
    init();

    mine::Program program;

    auto shaderProgram{mine::opengl::ShaderProgram::fromFiles(
        "shaders/vertex.glsl", "shaders/fragment.glsl")};

    mine::opengl::VertexArray vao{[](mine::opengl::VertexArray &vao) {
        // Square vertices
        float vertices[] = {
            0.5f,  0.5f,  0.0f, // top right
            0.5f,  -0.5f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f, 0.5f,  0.0f  // top left
        };

        unsigned int indices[] = {
            0, 1, 3, // first Triangle
            1, 2, 3  // second Triangle
        };

        auto &vbo{vao.addBuffer()};
        auto &ebo{vao.addBuffer(GL_ELEMENT_ARRAY_BUFFER)};

        vbo.bufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);
        ebo.bufferData(sizeof(indices), indices, GL_STATIC_DRAW);

        vao.vertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0);
    }};

    mine::Camera camera{0.3f, 0.01f};
    camera.setPosition({0.0f, 0.0f, -1.0f});

    while (program.isRunning()) {
        events(program, camera);
        render(program, vao, shaderProgram, camera);
    }

    return 0;
}
