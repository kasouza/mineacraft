#include "opengl/VertexArray.hpp"

#include <cassert>
#include <iostream>

namespace mine {

namespace opengl {

VertexArray::VertexArray() {
    glGenVertexArrays(1, &this->id);
    if (!this->id) {
        std::cerr << "Failed to create VAO" << std::endl;
        exit(1);
    }
}

VertexArray::VertexArray(std::function<void(VertexArray &)> setup) : VertexArray{} {
    this->bind();
    setup(*this);
    this->unbind();
}

VertexArray::VertexArray(VertexArray &&other) : id{other.id}, isBound{other.isBound} {
    other.id = 0;
    other.isBound = false;
}

void VertexArray::operator=(VertexArray &&other) {
    if (this->id) {
        glDeleteVertexArrays(1, &this->id);
    }

    this->id = other.id;
    this->isBound = other.isBound;

    other.id = 0;
    other.isBound = false;
}

VertexArray::~VertexArray() {
    if (this->id) {
        glDeleteVertexArrays(1, &this->id);
    }
}

void VertexArray::bind() {
    this->isBound = true;
    glBindVertexArray(this->id);
}

void VertexArray::unbind() {
    this->isBound = false;
    glBindVertexArray(0);

    for (auto &buffer : this->buffers) {
        buffer->unbind();
    }
}

void VertexArray::vertexAttribPointer(unsigned int index, int size, GLenum type,
                              bool normalized, int stride, void *pointer) {
    assert(this->isBound && "VAO is not bound");

    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);
}

GLBuffer &VertexArray::addBuffer(GLenum target) {
    this->buffers.emplace_back(std::make_unique<GLBuffer>(target));
    return *this->buffers.back();
}

GLBuffer &VertexArray::getBuffer(unsigned int index) {
    assert(index < this->buffers.size());
    return *this->buffers[index];
}

unsigned int VertexArray::get() { return this->id; }

} // namespace opengl

} // namespace mine
