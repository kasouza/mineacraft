#ifndef KASOUZA_MINECRAFT_INCLUDE_OPENGL_INCLUDE_OPENGL_GLBUFFER_HPP
#define KASOUZA_MINECRAFT_INCLUDE_OPENGL_INCLUDE_OPENGL_GLBUFFER_HPP

#include "opengl/gl_includes.hpp"

namespace mine {

namespace opengl {

/**
 * RAII wrapper for OpenGL Buffer Objects
 */
class GLBuffer {
  public:
    GLBuffer(const GLBuffer &other) = delete;
    void operator=(const GLBuffer &other) = delete;

    GLBuffer(GLenum _target = GL_ARRAY_BUFFER);

    GLBuffer(GLBuffer &&other) noexcept;

    void operator=(GLBuffer &&other) noexcept;

    ~GLBuffer();

    void bind();

    void unbind();

    void bufferData(GLsizeiptr size, const void *data, GLenum usage);

    unsigned int get();

  private:
    unsigned int id;
    bool isBound = false;
    GLenum target;
};

} // namespace opengl

} // namespace mine

#endif
