#include "opengl/gl_includes.hpp"
#include "opengl/Window.hpp"

#include <glm/fwd.hpp>
#include <glm/ext.hpp>

#include <cassert>
#include <iostream>

namespace mine {

namespace opengl {

Window::Window(Window &&other) : window{other.window} {
    other.window = nullptr;
}

void Window::operator=(Window &&other) {
    if (this->window) {
        glfwDestroyWindow(this->window);
    }
    this->window = other.window;
    other.window = nullptr;
}

Window::Window(const char *title, int width, int height) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    this->window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!this->window) {
        std::cerr << "Failed to create window" << std::endl;
        exit(1);
    }

    this->makeContextCurrent();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(1);
    }

    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    this->viewport(width, height);
}

Window::~Window() {
    if (this->window) {
        glfwDestroyWindow(this->window);
    }
}

void Window::makeContextCurrent() { glfwMakeContextCurrent(this->window); }

void Window::setFramebufferSizeCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(this->window, callback);
}

int Window::getWidth() const {
    int width{0};
    glfwGetWindowSize(this->window, &width, nullptr);

    return width;
}
int Window::getHeight() const {
    int height{0};
    glfwGetWindowSize(this->window, nullptr, &height);

    return height;
}

void Window::setWidth(int width) { this->viewport(width, this->getHeight()); }
void Window::setHeight(int height) { this->viewport(this->getWidth(), height); }

bool Window::shouldClose() const { return glfwWindowShouldClose(this->window); }

void Window::swapBuffers() { glfwSwapBuffers(this->window); }
void Window::pollEvents() { glfwPollEvents(); }

bool Window::isKeyPressed(int key) {
    return glfwGetKey(this->window, key) == GLFW_PRESS;
}

glm::vec2 Window::getCursorPos() {
    double cursorPosX, cursorPosY;
    glfwGetCursorPos(this->window, &cursorPosX, &cursorPosY);

    return {
        cursorPosX,
        cursorPosY,
    };
}

GLFWwindow *Window::get() { return this->window; }

void Window::viewport(int width, int height) {
    glViewport(0, 0, width, height);
}

} // namespace opengl

} // namespace mine
