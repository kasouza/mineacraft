#ifndef KASOUZA_MINECRAFT_INCLUDE_OPENGL_INCLUDE_OPENGL_SHADERPROGRAM_HPP
#define KASOUZA_MINECRAFT_INCLUDE_OPENGL_INCLUDE_OPENGL_SHADERPROGRAM_HPP

#include "glm/detail/qualifier.hpp"
#include "opengl/gl_includes.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <cstddef>
#include <string>

namespace mine {

namespace opengl {

/**
 * @brief Create a Shader object
 *
 * @param type GLenum
 * @param source const char*
 *
 * @return unsigned int
 */
unsigned int createShader(GLenum type, const char *source);

/**
 * @brief Create a Program object
 *
 * @param vertexShader const char*
 * @param fragmentShader const char*
 *
 * @return unsigned int
 */
unsigned int createProgram(const char *vertexShader,
                           const char *fragmentShader);

/**
 * @brief A RAII wrapper for OpenGL shader programs
 *
 */
class ShaderProgram {
  public:
    static ShaderProgram fromFiles(const char *vertexShaderPath,
                                   const char *fragmentShaderPath);

    ShaderProgram(const char *vertexShader, const char *fragmentShader);

    ShaderProgram(const ShaderProgram &) = delete;
    ShaderProgram &operator=(const ShaderProgram &) = delete;

    ShaderProgram(ShaderProgram &&other);
    void operator=(ShaderProgram &&other);

    ~ShaderProgram();

    void use();
    void unuse();

    unsigned int get();

    template <size_t N>
    void uniform(std::string name, glm::vec<N, float> value) {
        if constexpr (N == 1) {
            glUniform1fv(glGetUniformLocation(this->program, name.c_str()), 1,
                         glm::value_ptr(value));
        } else if (N == 2) {
            glUniform2fv(glGetUniformLocation(this->program, name.c_str()), 1,
                         glm::value_ptr(value));
        } else if (N == 3) {
            glUniform3fv(glGetUniformLocation(this->program, name.c_str()), 1,
                         glm::value_ptr(value));
        } else if (N == 4) {
            glUniform4fv(glGetUniformLocation(this->program, name.c_str()), 1,
                         glm::value_ptr(value));
        } else {
            static_assert(N == 1 || N == 2 || N == 3 || N == 4,
                          "Invalid vector size");
        }
    }

    template <size_t N> void uniform(std::string name, glm::vec<N, int> value) {
        if constexpr (N == 1) {
            glUniform1iv(glGetUniformLocation(this->program, name.c_str()), 1,
                         glm::value_ptr(value));
        } else if (N == 2) {
            glUniform2iv(glGetUniformLocation(this->program, name.c_str()), 1,
                         glm::value_ptr(value));
        } else if (N == 3) {
            glUniform3iv(glGetUniformLocation(this->program, name.c_str()), 1,
                         glm::value_ptr(value));
        } else if (N == 4) {
            glUniform4iv(glGetUniformLocation(this->program, name.c_str()), 1,
                         glm::value_ptr(value));
        } else {
            static_assert(N == 1 || N == 2 || N == 3 || N == 4,
                          "Invalid vector size");
        }
    }

    template <size_t N>
    void uniform(std::string name, glm::mat<N, N, float> value) {
        if constexpr (N == 2) {
            glUniformMatrix2fv(
                glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE,
                glm::value_ptr(value));
        } else if (N == 3) {
            glUniformMatrix3fv(
                glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE,
                glm::value_ptr(value));
        } else if (N == 4) {
            glUniformMatrix4fv(
                glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE,
                glm::value_ptr(value));
        } else {
            static_assert(N == 2 || N == 3 || N == 4, "Invalid matrix size");
        }
    }

    template <size_t N>
    void uniform(std::string name, glm::mat<N, N, int> value) {
        if constexpr (N == 2) {
            glUniformMatrix2iv(
                glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE,
                glm::value_ptr(value));
        } else if (N == 3) {
            glUniformMatrix3iv(
                glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE,
                glm::value_ptr(value));
        } else if (N == 4) {
            glUniformMatrix4iv(
                glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE,
                glm::value_ptr(value));
        } else {
            static_assert(N == 2 || N == 3 || N == 4, "Invalid matrix size");
        }
    }

  private:
    unsigned int program;
};

#endif
}
}
