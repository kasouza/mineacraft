#ifndef KASOUZA_MINECRAFT_INCLUDE_OPENGL_INCLUDE_OPENGL_VERTEXARRAY_HPP
#define KASOUZA_MINECRAFT_INCLUDE_OPENGL_INCLUDE_OPENGL_VERTEXARRAY_HPP

#include "opengl/GLBuffer.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace mine {

namespace opengl {

/**
 * RAII wrapper for OpenGL Vertex Array Objects
 */
class VertexArray {
  public:
    VertexArray(const VertexArray &other) = delete;
    void operator=(const VertexArray &other) = delete;

    VertexArray();
    VertexArray(std::function<void(VertexArray &)> setup);

    VertexArray(VertexArray &&other);
    void operator=(VertexArray &&other);

    ~VertexArray();

    void bind();

    void unbind();

    void vertexAttribPointer(unsigned int index, int size, GLenum type,
                             bool normalized, int stride, void *pointer);

    GLBuffer &addBuffer(GLenum target = GL_ARRAY_BUFFER);

    GLBuffer &getBuffer(unsigned int index);

    unsigned int get();

  private:
    unsigned int id;
    bool isBound = false;
    std::vector<std::unique_ptr<GLBuffer>> buffers;
};

} // namespace opengl

} // namespace mine

#endif
