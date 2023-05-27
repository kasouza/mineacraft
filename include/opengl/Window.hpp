#ifndef KASOUZA_MINECRAFT_INCLUDE_OPENGL_INCLUDE_OPENGL_WINDOW_HPP
#define KASOUZA_MINECRAFT_INCLUDE_OPENGL_INCLUDE_OPENGL_WINDOW_HPP

#include "opengl/gl_includes.hpp"

#include <glm/ext.hpp>

namespace mine {

namespace opengl {

/**
 * A wrapper around GLFWwindow
 */
class Window {
  public:
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    Window(Window &&other);

    void operator=(Window &&other);

    Window(const char *title = "Hello World", int width = 640,
           int height = 480);

    ~Window();

    void makeContextCurrent();

    void setFramebufferSizeCallback(GLFWframebuffersizefun callback);

    int getWidth() const;
    int getHeight() const;

    void setWidth(int width);
    void setHeight(int height);

    bool shouldClose() const;

    void swapBuffers();
    void pollEvents();

    bool isKeyPressed(int key);
    glm::vec2 getCursorPos();

    GLFWwindow *get();

  private:
    GLFWwindow *window;

    void viewport(int width, int height);
};

} // namespace opengl

} // namespace mine

#endif
