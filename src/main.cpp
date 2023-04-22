#include <cassert>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>
#include <sstream>
#include <vector>

#include "gl_includes.hpp"

struct Program {
    bool running;
    GLFWwindow *window;
};

void clean() { glfwTerminate(); }

GLFWwindow *createWindow(int width = 640, int height = 480,
                         const char *title = "Hello, World") {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        exit(1);
    }

    return window;
}

Program init() {
    atexit(clean);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(1);
    }

    int width = 640;
    int height = 480;

    GLFWwindow *window = createWindow(width, height);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(1);
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow *, int newWidth, int newHeight) {
            glViewport(0, 0, newWidth, newHeight);
        });

    return {true, window};
}

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

/**
 * Read a file and return its contents as a string
 *
 * @param path const char*
 * @return std::string
 */
std::string readFile(const char *path) {
    std::fstream fs{path};
    if (fs.fail()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        exit(1);
    }

    std::stringstream ss;
    ss << fs.rdbuf();

    return ss.str();
}

/**
 * @brief A RAII wrapper for OpenGL shader programs
 *
 */
class ShaderProgram {
  public:
    static ShaderProgram fromFiles(const char *vertexShaderPath,
                                   const char *fragmentShaderPath) {
        std::string vertexShader = readFile(vertexShaderPath);
        std::string fragmentShader = readFile(fragmentShaderPath);

        return {vertexShader.c_str(), fragmentShader.c_str()};
    }

    ShaderProgram(const char *vertexShader, const char *fragmentShader) {
        this->program = createProgram(vertexShader, fragmentShader);
    }

    ShaderProgram(const ShaderProgram &) = delete;
    ShaderProgram &operator=(const ShaderProgram &) = delete;

    ShaderProgram(ShaderProgram &&other) {
        this->program = other.program;
        other.program = 0;
    }

    void operator=(ShaderProgram &&other) {
        this->program = other.program;
        other.program = 0;
    }

    ~ShaderProgram() {
        if (this->program) {
            glDeleteProgram(this->program);
        }
    }

    void use() { glUseProgram(this->program); }
    void unuse() { glUseProgram(0); }

    inline unsigned int get() { return this->program; }

  private:
    unsigned int program;
};

/**
 * RAII wrapper for OpenGL Buffer Objects
 */
class GLBuffer {
  public:
    GLBuffer(const GLBuffer &other) = delete;
    void operator=(const GLBuffer &other) = delete;

    GLBuffer(GLenum _target = GL_ARRAY_BUFFER) : target{_target} {
        glGenBuffers(1, &this->id);
        if (!this->id) {
            std::cerr << "Failed to create VBO" << std::endl;
            exit(1);
        }
    }

    GLBuffer(GLBuffer &&other) noexcept {
        this->id = other.id;
        this->target = other.target;
        this->isBound = other.isBound;

        other.id = 0;
    }

    ~GLBuffer() {
        if (this->id) {
            glDeleteBuffers(1, &this->id);
        }
    }

    void operator=(GLBuffer &&other) noexcept {
        this->id = other.id;
        this->target = other.target;

        other.id = 0;
    }

    void bind() {
        assert(this->id);
        this->isBound = true;
        glBindBuffer(this->target, this->id);
    }

    void unbind() {
        this->isBound = false;
        glBindBuffer(this->target, 0);
    }

    void bufferData(GLsizeiptr size, const void *data, GLenum usage) {
        assert(this->id);

        if (!this->isBound) {
            this->bind();
        }

        glBufferData(this->target, size, data, usage);
    }

    unsigned int get() { return this->id; }

  private:
    unsigned int id;
    bool isBound = false;
    GLenum target;
};

/**
 * RAII wrapper for OpenGL Vertex Array Objects
 */
class VAO {
  public:
    VAO(const VAO &other) = delete;
    void operator=(const VAO &other) = delete;

    VAO() {
        glGenVertexArrays(1, &this->id);
        if (!this->id) {
            std::cerr << "Failed to create VAO" << std::endl;
            exit(1);
        }
    }

    VAO(std::function<void(VAO &)> setup) : VAO{} {
        this->bind();
        setup(*this);
        this->unbind();
    }

    VAO(VAO &&other) {
        this->id = other.id;
        this->isBound = other.isBound;

        other.id = 0;
        other.isBound = false;
    }

    ~VAO() {
        if (this->id) {
            glDeleteVertexArrays(1, &this->id);
        }
    }

    void operator=(VAO &&other) {
        this->id = other.id;
        this->isBound = other.isBound;

        other.id = 0;
        other.isBound = false;
    }

    void bind() {
        this->isBound = true;
        glBindVertexArray(this->id);
    }

    void unbind() {
        this->isBound = false;
        glBindVertexArray(0);

        for (auto &buffer : this->buffers) {
            buffer->unbind();
        }
    }

    void vertexAttribPointer(unsigned int index, int size, GLenum type,
                             bool normalized, int stride, void *pointer) {
        assert(this->isBound && "VAO is not bound");

        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        glEnableVertexAttribArray(index);
    }

    GLBuffer &addBuffer(GLenum target = GL_ARRAY_BUFFER) {
        this->buffers.emplace_back(std::make_unique<GLBuffer>(target));
        return *this->buffers.back();
    }

    GLBuffer &getBuffer(unsigned int index) {
        assert(index < this->buffers.size());
        return *this->buffers[index];
    }

    unsigned int get() { return this->id; }

  private:
    unsigned int id;
    bool isBound = false;
    std::vector<std::unique_ptr<GLBuffer>> buffers;
};

inline bool isKeyPressed(GLFWwindow *window, int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void events(Program &program) {
    glfwPollEvents();

    if (isKeyPressed(program.window, GLFW_KEY_ESCAPE) ||
        glfwWindowShouldClose(program.window)) {
        program.running = false;
    }
}

inline void clearScreen() {
    glClearColor(0.2f, 0.3f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void render(Program &program, VAO &vao, ShaderProgram &shaderProgram) {
    clearScreen();

    shaderProgram.use();
    vao.bind();

    // Draw the square
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    shaderProgram.unuse();

    vao.unbind();

    glfwSwapBuffers(program.window);
}

int main() {
    Program program = init();

    ShaderProgram shaderProgram = ShaderProgram::fromFiles(
        "shaders/vertex.glsl", "shaders/fragment.glsl");

    VAO vao{[](VAO &vao) {
        // Square vertices
        float vertices[] = {
            0.5f,  0.5f,  0.0f, // top right
            0.5f,  -0.5f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f, 0.5f,  0.0f  // top left
        };

        unsigned int indices[] = {
            // note that we start from 0!
            0, 1, 3, // first Triangle
            1, 2, 3  // second Triangle
        };

        GLBuffer &vbo{vao.addBuffer()};
        GLBuffer &ebo{vao.addBuffer(GL_ELEMENT_ARRAY_BUFFER)};

        vbo.bufferData(sizeof(vertices), vertices, GL_STATIC_DRAW);
        ebo.bufferData(sizeof(indices), indices, GL_STATIC_DRAW);

        vao.vertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0);
    }};

    while (program.running) {
        events(program);
        render(program, vao, shaderProgram);
    }

    return 0;
}
