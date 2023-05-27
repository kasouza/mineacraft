#ifndef KASOUZA_MINECRAFT_INCLUDE_INCLUDE_PROGRAM_HPP
#define KASOUZA_MINECRAFT_INCLUDE_INCLUDE_PROGRAM_HPP

#include "opengl/Window.hpp"

namespace mine {

/**
 * Program class
 *
 * It is responsible for initializing GLAD and for creating the window.
 */
class Program {
  public:
    Program(const char *title = "Hello, World!", int width = 640,
            int height = 480);

    Program(const Program &other) = delete;
    void operator=(const Program &other) = delete;

    Program(Program &&other);
    void operator=(Program &&other);

    bool isRunning() const;
    void stop();

    opengl::Window &getWindow();

  private:
    bool running;
    opengl::Window window;
};

}; // namespace mine

#endif
