file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    main.cpp
    opengl/ShaderProgram.cpp
    opengl/VertexArray.cpp
    opengl/GLBuffer.cpp
    opengl/Window.cpp
    Program.cpp
    utils/fs.cpp
    Camera.cpp
)

set(LIBS
    glfw
    glad
    stb_image
    glm
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wpedantic)
