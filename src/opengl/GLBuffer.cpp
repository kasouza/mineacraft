#include "opengl/GLBuffer.hpp"

#include <cassert>
#include <iostream>

namespace mine {

namespace opengl {

GLBuffer::GLBuffer(GLenum _target) : target{_target} {
    glGenBuffers(1, &this->id);
    if (!this->id) {
        std::cerr << "Failed to create VBO" << std::endl;
        exit(1);
    }
}

GLBuffer::GLBuffer(GLBuffer &&other) noexcept
    : id{other.id}, isBound{other.isBound}, target{other.target} {
    other.id = 0;
}

void GLBuffer::operator=(GLBuffer &&other) noexcept {
    if (this->id) {
        glDeleteBuffers(1, &this->id);
    }

    this->id = other.id;
    this->target = other.target;

    other.id = 0;
}

GLBuffer::~GLBuffer() {
    if (this->id) {
        glDeleteBuffers(1, &this->id);
    }
}

void GLBuffer::bind() {
    assert(this->id);
    this->isBound = true;
    glBindBuffer(this->target, this->id);
}

void GLBuffer::unbind() {
    assert(this->id);
    this->isBound = false;
    glBindBuffer(this->target, 0);
}

void GLBuffer::bufferData(GLsizeiptr size, const void *data, GLenum usage) {
    assert(this->id);
    if (!this->isBound) {
        this->bind();
    }

    glBufferData(this->target, size, data, usage);
}

unsigned int GLBuffer::get() {
    assert(this->id);
    return this->id;
}

} // namespace opengl

} // namespace mine
