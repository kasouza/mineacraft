#include "opengl/ShaderProgram.hpp"
#include "utils/fs.hpp"

#include <iostream>

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
unsigned int createShader(GLenum type, const char *source) {
    unsigned int shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Failed to compile shader: " << infoLog << std::endl;
        exit(1);
    }

    return shader;
}

/**
 * @brief Create a Program object
 *
 * @param vertexShader const char*
 * @param fragmentShader const char*
 *
 * @return unsigned int
 */
unsigned int createProgram(const char *vertexShader,
                           const char *fragmentShader) {
    unsigned int program = glCreateProgram();

    unsigned int vertex = createShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragment = createShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Failed to link program: " << infoLog << std::endl;
        exit(1);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

ShaderProgram ShaderProgram::fromFiles(const char *vertexShaderPath,
                                       const char *fragmentShaderPath) {
    using mine::utils::fs::readFile;

    std::string vertexShader = readFile(vertexShaderPath);
    std::string fragmentShader = readFile(fragmentShaderPath);

    return {vertexShader.c_str(), fragmentShader.c_str()};
}

ShaderProgram::ShaderProgram(const char *vertexShader,
                             const char *fragmentShader)
    : program{createProgram(vertexShader, fragmentShader)} {}

ShaderProgram::ShaderProgram(ShaderProgram &&other) : program{other.program} {
    other.program = 0;
}

void ShaderProgram::operator=(ShaderProgram &&other) {
    if (this->program) {
        glDeleteProgram(this->program);
    }

    this->program = other.program;
    other.program = 0;
}

ShaderProgram::~ShaderProgram() {
    if (this->program) {
        glDeleteProgram(this->program);
    }
}

void ShaderProgram::use() { glUseProgram(this->program); }
void ShaderProgram::unuse() { glUseProgram(0); }

unsigned int ShaderProgram::get() { return this->program; }

} // namespace opengl

} // namespace mine
