#include "Program.hpp"

#include <memory>

namespace mine {

Program::Program(const char *title, int width, int height)
    : running{true}, window{title, width, height} {
    this->window.setFramebufferSizeCallback(
        [](GLFWwindow *, int newWidth, int newHeight) {
            glViewport(0, 0, newWidth, newHeight);
        });
}

Program::Program(Program &&other)
    : running{other.running}, window{std::move(other.window)} {
    other.running = false;
}

void Program::operator=(Program &&other) {
    this->running = other.running;
    this->window = std::move(other.window);

    other.running = false;
}

bool Program::isRunning() const { return this->running; }

void Program::stop() {
    this->running = false;
    glfwSetWindowShouldClose(this->window.get(), true);
}

opengl::Window &Program::getWindow() { return this->window; }

} // namespace mine
